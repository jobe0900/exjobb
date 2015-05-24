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
"           (ST_Azimuth(ST_PointN(geom,1), "
"                       ST_PointN(geom,2))/(2*pi())*360)::int "
"                       AS source_bearing, "
"           (ST_Azimuth(ST_PointN(geom,ST_NPoints(geom)-1), "
"                       ST_PointN(geom,ST_NPoints(geom)))/(2*pi())*360)::int "
"                       AS target_bearing, "
//-- osm data about original edge
"           osm.* "
"FROM       topo_test.edge_data "
"JOIN ( "
"   SELECT  osm_id, element_id "
//-- road data
"           , highway "
"           , junction "
"           , lanes "
"           , oneway "
//-- vehicle property restrictions
"           , maxheight "
"           , maxlength "
"           , maxspeed "
"           , maxweight "
"           , maxwidth "
"           , minspeed "
//-- access restrictions
"           , access "
"           , motorcar "
"           , goods "
"           , hgv "
"           , psv "
"           , lhv "
"           , motor_vehicle "
"           , vehicle "
//-- access
//"           , barrier "
//"           , disused "
//"           , emergency "
//"           , noexit "
//"           , restriction "
//-- costs
//"           , incline "
//"           , public_transport "
//"           , railway "
//"           , surface "
//"           , tracktype "
//"           , traffic_calming "
//"           , traffic_sign "
"   FROM    " + mSchemaName + ".relation "
"   JOIN    " + mTableName +
"   ON      topogeo_id = (topo_geom).id "
"   WHERE   highway in " + getInterestingHighwayColumns() +
") AS osm "
"ON edge_id = element_id "
"ORDER BY edge_id ASC;"
		    );
		rEdgeResult = non_trans.exec(sql);
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
    for(const pqxx::tuple& row : result)
    {
        if(!validDimensionRestrictions(row))
        {
            continue;
        }

        if(!accessAllowed(row))
        {
            continue;
        }
        Edge& edge = addBasicResultToEdge(row, rTopology);
        addGeomDataResultToEdge(edge, row);
        addRoadDataResultToEdge(edge, row);
    }
}

bool
PostGisProvider::validDimensionRestrictions(const pqxx::tuple& rRow) const
{
    const VehicleConfig& config = mrConfig.getVehicleConfig();
    const double doubleMax(std::numeric_limits<double>::max());

    using namespace OsmConstants;

    if(config.height >= rRow[EdgeResultColumns::MAXHEIGHT_COL].as<double>(doubleMax))
        return false;
    if(config.length >= rRow[EdgeResultColumns::MAXLENGTH_COL].as<double>(doubleMax))
        return false;
    if(config.weight >= rRow[EdgeResultColumns::MAXWEIGHT_COL].as<double>(doubleMax))
        return false;
    if(config.width  >= rRow[EdgeResultColumns::MAXWIDTH_COL].as<double>(doubleMax))
        return false;

    return true;
}

bool
PostGisProvider::accessAllowed(const pqxx::tuple& rRow) const
{
    using namespace OsmConstants;
    AccessType generic_access = genericAccess(rRow);
    AccessType type_access = vehicleTypeAccessType(rRow);

    // generic allowed, but type restricted
    if(generic_access == AccessType::ALLOWED)
    {
        if(type_access == AccessType::RESTRICTED)
        {
            return false;
        }
        return true;
    }
    // generic restricted, but type allowed
    else if(generic_access == AccessType::RESTRICTED)
    {
        if(type_access == AccessType::ALLOWED)
        {
            return true;
        }
        return false;
    }
    // generic unspecified, but type restricted
    else if(type_access == AccessType::RESTRICTED)
    {
        return false;
    }
    // generic and type unspecified == allowed
    return true;
}

OsmConstants::AccessType
PostGisProvider::genericAccess(const pqxx::tuple& rRow) const
{
    using namespace OsmConstants;
    return accessTypeFromRestrictionString(rRow[ACCESS_COL].as<std::string>(""));
}

OsmConstants::AccessType
PostGisProvider::vehicleTypeAccessType(const pqxx::tuple& rRow) const
{
    using namespace OsmConstants;

    const VehicleType& v_type = mrConfig.getVehicleConfig().category;

    std::string restriction_string("");
    switch(v_type)
    {
        case VEHICLE_MOTORCAR:
            restriction_string =
                rRow[EdgeResultColumns::MOTORCAR_COL].as<std::string>(""); break;
        case VEHICLE_GOODS:
            restriction_string =
                rRow[EdgeResultColumns::GOODS_COL].as<std::string>(""); break;
        case VEHICLE_HGV:
            restriction_string =
                rRow[EdgeResultColumns::HGV_COL].as<std::string>(""); break;
        case VEHICLE_PSV:
            restriction_string =
                rRow[EdgeResultColumns::PSV_COL].as<std::string>(""); break;
        case VEHICLE_LHV:
            restriction_string =
                rRow[EdgeResultColumns::LHV_COL].as<std::string>(""); break;
        case VEHICLE_MOTOR_VEHICLE:
            restriction_string =
                rRow[EdgeResultColumns::MOTOR_VEHICLE_COL].as<std::string>(""); break;
        case VEHICLE_VEHICLE:
        default:
            restriction_string =
                rRow[EdgeResultColumns::VEHICLE_COL].as<std::string>(""); break;

    }
    return accessTypeFromRestrictionString(restriction_string);
}

OsmConstants::AccessType
PostGisProvider::accessTypeFromRestrictionString(
        const std::string& rRestrictionString) const
{
    using namespace OsmConstants;

    if(rRestrictionString != "")
    {
        if( rRestrictionString == ACCESS_YES ||
            rRestrictionString == ACCESS_PERMISSIVE ||
            rRestrictionString == ACCESS_DESIGNATED)
        {
            return ALLOWED;
        }
        else
        {
            return RESTRICTED;
        }
    }
    return NOT_SPECIFIED;
}

Edge&
PostGisProvider::addBasicResultToEdge(const pqxx::tuple& rRow,
                                      Topology& rTopology)
{
    using namespace OsmConstants;
    EdgeIdType    edge_id(rRow[EDGE_ID_COL].as<EdgeIdType>(
                        std::numeric_limits<EdgeIdType>::max()));
    OsmIdType     osm_id(rRow[OSM_ID_COL].as<OsmIdType>(
                        std::numeric_limits<OsmIdType>::max()));
    VertexIdType  source_id(rRow[START_NODE_COL].as<int>(
                        std::numeric_limits<VertexIdType>::max()));
    VertexIdType  target_id(rRow[END_NODE_COL].as<int>(
                        std::numeric_limits<VertexIdType>::max()));
    Edge&         edge = rTopology.addEdge(
                        edge_id, osm_id, source_id, target_id);

    return edge;
}

void
PostGisProvider::addGeomDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    using namespace OsmConstants;
    Edge::GeomData  gd(rRow[EDGE_LENGTH_COL].as<double>(0),
                       Point(rRow[CENTER_X_COL].as<double>(0),
                             rRow[CENTER_Y_COL].as<double>(0)),
                       rRow[SOURCE_BEARING_COL].as<int>(0),
                       rRow[TARGET_BEARING_COL].as<int>(0));
    rEdge.setGeomData(gd);
}

void
PostGisProvider::addRoadDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    using namespace OsmConstants;
    Edge::RoadData rd;
    std::string onewayStr(rRow[ONEWAY_COL].as<std::string>("no"));

    if(rRow[JUNCTION_COL].as<std::string>("") == ROUNDABOUT)
    {
        onewayStr = "yes";
    }
    if(onewayStr == "yes")
    {
        rd.direction = Edge::DirectionType::FROM_TO;
    }
    else if(onewayStr == "-1")
    {
        rd.direction = Edge::DirectionType::TO_FROM;
    }

    rd.nrLanes = rRow[LANES_COL].as<size_t>(1);

    addHighwayTypeToEdgeRoadData(rd, rRow);

    rEdge.setRoadData(rd);
}

void
PostGisProvider::addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                              const pqxx::tuple& rRow)
{
    using namespace OsmConstants;
    std::string roadTypeStr(rRow[HIGHWAY_COL].as<std::string>("road"));

    for(size_t i = 0; i < HighwayType::NR_HIGHWAY_TYPES; ++i)
    {
        if(roadTypeStr == HighwayTypeStrings[i])
        {
            rRoadData.roadType = static_cast<HighwayType>(i);
            return;
        }
    }
}

std::string
PostGisProvider::getInterestingHighwayColumns() const
{
    using namespace OsmConstants;
    std::string cols;
    std::stringstream ss;
    ss << "(";
    for(size_t i = 0; i < NR_HIGHWAY_TYPES; ++i)
    {
        ss << "'" << HighwayTypeStrings[i] << "'";
        if(i < NR_HIGHWAY_TYPES - 1)
        {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
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

