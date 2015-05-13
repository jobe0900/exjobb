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
PostGisProvider::PostGisProvider(const std::string& rTopoName,
                                 const DatabaseConfig& rDatabaseConfig)
try		// catch error when initializing connection
	:   mTopoName(rTopoName),
        mDbConfig(rDatabaseConfig),
        mConnection(mDbConfig.getConnectionString())
{
	try
	{
		if(!mConnection.is_open())
		{
			throw PostGisProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}
		pqxx::nontransaction nt(mConnection);
		mTableName = (TEMP_TABLE_PREFIX + nt.esc(mTopoName));
		mSchemaName = (TEMP_SCHEMA_PREFIX + nt.esc(mTopoName));
	}
	catch(const std::exception& e)
	{
		throw PostGisProviderException(std::string("Database error, open: ") + e.what());
	}
}
// catch error in initializer list (opening connection)
catch(const std::exception& e)
{
	throw PostGisProviderException(std::string("Database error, open: ") + e.what());
}


PostGisProvider::~PostGisProvider()
{
	try
	{
		if(mConnection.is_open())
		{
			mConnection.disconnect();
		}
	}
	catch(const std::exception& e)
	{
		//swallow errors
//		throw PostGisProviderException(std::string("Database error, closing: ") + e.what());
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================

void
PostGisProvider::buildTopology(int srid, double tolerance)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw PostGisProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}


		// TRANSACTION START
		pqxx::work transaction(mConnection);

		installPostgisTopology(transaction);
		setSearchPath(transaction);
		createTemporaryTable(transaction, mTableName);
		createTemporarySchema(transaction, mSchemaName, srid);
		addTopoGeometryColumn(transaction, mSchemaName, mTableName);
		fillTopoGeometryColumn(transaction, mSchemaName, mTableName, tolerance);

		// TRANSACTION END
		transaction.commit();
	}
	catch(const std::exception& e)
	{
		throw PostGisProviderException(std::string("Database error: ") + e.what());
	}
}


void
PostGisProvider::removeTopology()
{
	try
	{
		if(!mConnection.is_open())
		{
			throw PostGisProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// TRANSACTION START
		pqxx::work transaction(mConnection);

		dropTemporaryTable(transaction, mTableName);
		dropTemporarySchema(transaction, mSchemaName);
		deleteTemporaryLayerRecord(transaction, mTableName);
		deleteTemporaryTopoRecord(transaction, mSchemaName);

		// TRANSACTION END
		transaction.commit();
	}
	catch(const std::exception& e)
	{
		throw PostGisProviderException(std::string("Database error: ") + e.what());
	}

}


void
PostGisProvider::getTopologyVertices(std::map<VertexId, TopologyVertex>& rVertexMap)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw PostGisProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		pqxx::result result = non_trans.exec(
				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
				"FROM " + mSchemaName + ".node "
				"ORDER BY node_id ASC;"
		);

		for(size_t row = 0; row < result.size(); ++row)
		{
			VertexId	id(result[row][0].as<int>());
			Point 		p(result[row][1].as<double>(), result[row][2].as<double>());
			// TODO check if emplace is succesful?
			rVertexMap.emplace(id, TopologyVertex(id, p));
		}
	}
	catch(const std::exception& e)
	{
		throw PostGisProviderException(std::string("Database error: ") + e.what());
	}
}


void
PostGisProvider::getTopologyEdges(std::map<EdgeId, TopologyEdge>& rEdgeMap)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw PostGisProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		pqxx::result result = non_trans.exec(
				"SELECT edge_id, start_node, end_node "
				"FROM " + mSchemaName + ".edge_data "
				"ORDER BY edge_id ASC;"
		);

		for(size_t row = 0; row < result.size(); ++row)
		{
			EdgeId 		edge_id(result[row][0].as<int>());
			VertexId 	source_id(result[row][1].as<int>());
			VertexId 	target_id(result[row][2].as<int>());
			rEdgeMap.emplace(edge_id, TopologyEdge(edge_id, source_id, target_id));
		}
	}
	catch(const std::exception& e)
	{
		throw PostGisProviderException(std::string("Database error: ") + e.what());
	}
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
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

