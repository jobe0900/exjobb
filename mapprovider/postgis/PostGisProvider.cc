/*
 * PostGisProvider.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../postgis/PostGisProvider.h"  // class implemented

#include <iostream>
#include <sstream>



/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
PostGisProvider::PostGisProvider(const Configuration& rConfig)
try
    : MapProvider(rConfig),
      mDbConfig(rConfig.getDatabaseConfig()),
      mTopoConfig(rConfig.getTopologyConfig()),
      mConnection(mDbConfig.getConnectionString())
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        std::string topoBaseName;
        setTopoBaseName(topoBaseName);

        if(topoBaseName == "")
        {
            throw MapProviderException("No topology specified.");
        }

        pqxx::nontransaction nt(mConnection);
        mTableName  = nt.esc(mTopoConfig.roadsPrefix + "_" + topoBaseName);
        mSchemaName = nt.esc(mTopoConfig.topologySchemaPrefix + "_" + topoBaseName);
        nt.abort();

        if(mBuildTempTopology)
        {
            buildTopology(mTopoConfig.srid, mTopoConfig.tolerance);
        }
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:ctor(in): ") + e.what());
    }
}
// catch error in initializer list (opening connection)
catch(const std::exception& e)
{
        throw MapProviderException(
            std::string("PostGisProvider:ctor(out): ") + e.what());
}



PostGisProvider::~PostGisProvider()
{
	try
	{
	    if(mBuildTempTopology)
	    {
	        removeTopology();
	    }
		if(mConnection.is_open())
		{
			mConnection.disconnect();
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:dtor: ") + e.what());
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
void
PostGisProvider::getTopology(Topology& rTopology)
{
    pqxx::result vertex_result;
    getTopologyVertices(vertex_result);
    addVertexResultToTopology(vertex_result, rTopology);

    pqxx::result edge_result;
    getTopologyEdges(edge_result);
    addEdgeResultToTopology(edge_result, rTopology);
}


//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

void
PostGisProvider::getTopologyVertices(pqxx::result& rVertexResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		rVertexResult = non_trans.exec(
				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
				"FROM " + mSchemaName + ".node "
				"ORDER BY node_id ASC;"
		);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopoVertices: ") + e.what());
	}
}

void
PostGisProvider::addVertexResultToTopology(const pqxx::result& result,
                                           Topology& rTopology)
{
    for(size_t row = 0; row < result.size(); ++row)
    {
        VertexIdType	id(result[row][0].as<int>());
        Point 		    point(result[row][1].as<double>(),
                              result[row][2].as<double>());
        rTopology.addVertex(id, point);
    }
}


void
PostGisProvider::getTopologyEdges(pqxx::result& rEdgeResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		std::string sql(
"SELECT     edge_id, "
"           start_node, "
"           end_node, "
//-- geom data about edge
"           ST_Length(geom) AS edge_length, "
"           ST_X(ST_LineInterpolatePoint(geom, 0.5)) AS center_x, "
"           ST_Y(ST_LineInterpolatePoint(geom, 0.5)) AS center_y, "
"           ST_Azimuth(ST_PointN(geom,1), "
"                      ST_PointN(geom,2))/(2*pi())*360 "
"                      AS source_bearing, "
"           ST_Azimuth(ST_PointN(geom,ST_NPoints(geom)-1), "
"                      ST_PointN(geom,ST_NPoints(geom)))/(2*pi())*360 "
"                      AS target_bearing, "
//-- osm data about original edge
"           osm.* "
"FROM       topo_test.edge_data "
"JOIN ( "
"   SELECT  osm_id, element_id, "
//-- road data
"           highway, "
"           junction, "
"           lanes, "
"           oneway, "
//-- access
"           access, "
"           barrier, "
"           disused, "
"           emergency, "
//-- restrictions
"           maxheight, "
"           maxlength, "
"           maxspeed, "
"           maxweight,"
"           maxwidth, "
"           minspeed, "
"           noexit, "
"           restriction, "
//-- vehicle access restrictions
"           goods, "
"           hgv, "
"           lhv, "
"           motorcar, "
"           motor_vehicle, "
"           psv, "
"           vehicle, "
//-- costs
"           incline, "
"           public_transport, "
"           railway, "
"           surface, "
"           tracktype, "
"           traffic_calming, "
"           traffic_sign "
"   FROM    topo_test.relation "
"   JOIN    public.highways_test"
"   ON      topogeo_id = (topo_geom).id "
") AS osm "
"ON edge_id = element_id "
"ORDER BY edge_id ASC;"
		    );
		rEdgeResult = non_trans.exec(sql);
//		rEdgeResult = non_trans.exec(
//				"SELECT edge_id, start_node, end_node "
//				"FROM " + mSchemaName + ".edge_data "
//				"ORDER BY edge_id ASC;"
//		);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopoEdges: ") + e.what());
	}
}

void
PostGisProvider::addEdgeResultToTopology(const pqxx::result& result,
                                         Topology& rTopology)
{
    for(size_t row = 0; row < result.size(); ++row)
    {
        EdgeIdType      edge_id(result[row][0].as<int>());
        VertexIdType    source_id(result[row][1].as<int>());
        VertexIdType    target_id(result[row][2].as<int>());;
        rTopology.addEdge(edge_id, source_id, target_id);
    }
}

void
PostGisProvider::buildTopology(int srid, double tolerance)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}


		// TRANSACTION START
		pqxx::work transaction(mConnection);

		try
		{
		    installPostgisTopology(transaction);
		    setSearchPath(transaction);
		    createTemporaryTable(transaction, mTableName);
		    createTemporarySchema(transaction, mSchemaName, srid);
		    addTopoGeometryColumn(transaction, mSchemaName, mTableName);
		    fillTopoGeometryColumn(transaction, mSchemaName, mTableName, tolerance);

		    // TRANSACTION END
		    transaction.commit();
		}
		catch (const std::exception& e)
		{
		    transaction.abort();
		    throw e;
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:buildTopology: ") + e.what());
	}
}


void
PostGisProvider::removeTopology()
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// TRANSACTION START
		pqxx::work transaction(mConnection);

		try
		{
		    dropTemporaryTable(transaction, mTableName);
		    dropTemporarySchema(transaction, mSchemaName);
		    deleteTemporaryLayerRecord(transaction, mTableName);
		    deleteTemporaryTopoRecord(transaction, mSchemaName);

		    // TRANSACTION END
		    transaction.commit();
		}
		catch (const std::exception& e)
		{
		    transaction.abort();
		    throw e;
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(std::string(
            "PostGisProvider:removeTopology: ") + e.what());
	}

}

void
PostGisProvider::setTopoBaseName(std::string& rTopoBaseName)
{
    if(mTopoConfig.tempTopoName == TopologyConfig::TEMP_TOPO_NAMEBASE)
    {
        rTopoBaseName = TimeToStringMaker::getEpochMsTimeString();
        mBuildTempTopology = true;
    }
    else
    {
        rTopoBaseName = mTopoConfig.topoName;
    }
}

void
PostGisProvider::installPostgisTopology(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"CREATE EXTENSION IF NOT EXISTS postgis_topology"
	);
}

void
PostGisProvider::setSearchPath(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"SET search_path = topology, public"
	);
}

void
PostGisProvider::createTemporaryTable(pqxx::transaction_base& rTrans,
									  const std::string& rTableName)
{
	rTrans.exec(
			"CREATE TABLE public." + rTableName + " " +
			"AS SELECT * "
			"FROM planet_osm_line "
			"WHERE highway IS NOT NULL"
	);
}

void
PostGisProvider::createTemporarySchema(pqxx::transaction_base& rTrans,
									  const std::string& rSchemaName, int srid)
{
	rTrans.exec(
			"SELECT topology.CreateTopology('" +
			rSchemaName + "'," +
			rTrans.quote(srid) + ")"
	);
}

void
PostGisProvider::addTopoGeometryColumn(pqxx::transaction_base& rTrans,
									   const std::string& rSchemaName,
									   const std::string& rTableName)
{
	rTrans.exec(
			"SELECT topology.AddTopoGeometryColumn('" +
			rSchemaName + "', " +
			"'public', '" +
			rTableName + "', " +
			"'topo_geom', 'LINESTRING')"
	);
}

void
PostGisProvider::fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
									    const std::string& rSchemaName,
									    const std::string& rTableName,
										double tolerance)
{
	rTrans.exec(
			"UPDATE public." +
			rTableName + " " +
			"SET topo_geom = topology.toTopoGeom(way, '" +
			rSchemaName +
			"', 1, " +
			rTrans.quote(tolerance) + ")"
	);
}

void
PostGisProvider::dropTemporaryTable(pqxx::transaction_base& rTrans,
									const std::string& rTableName)
{
	rTrans.exec(
			"DROP TABLE IF EXISTS public." + rTableName
	);
}

void
PostGisProvider::dropTemporarySchema(pqxx::transaction_base& rTrans,
									 const std::string& rSchemaName)
{
	rTrans.exec(
			"DROP SCHEMA IF EXISTS " + rSchemaName + " CASCADE"
	);
}

void
PostGisProvider::deleteTemporaryLayerRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rTableName)
{
	rTrans.exec(
			"DELETE FROM topology.layer "
			"WHERE table_name = " + rTrans.quote(rTableName)
	);
}

void
PostGisProvider::deleteTemporaryTopoRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rSchemaName)
{
	rTrans.exec(
			"DELETE FROM topology.topology "
			"WHERE name = " + rTrans.quote(rSchemaName)
	);
}

