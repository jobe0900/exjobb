/*
 * DatabaseHandler.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include <iostream>
#include <sstream>

#include "DatabaseHandler.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
DatabaseHandler::DatabaseHandler(const DatabaseConfig& rDatabaseConfig)
try		// catch error when initializing connection
	: mDbConfig(rDatabaseConfig), mConnection(mDbConfig.getConnectionString())
{
	try
	{
		if(!mConnection.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}
	}
	catch(const std::exception& e)
	{
		throw DatabaseException(std::string("Database error, open: ") + e.what());
	}
}
// catch error in initializer list (opening connection)
catch(const std::exception& e)
{
	throw DatabaseException(std::string("Database error, open: ") + e.what());
}


DatabaseHandler::~DatabaseHandler()
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
//		throw DatabaseException(std::string("Database error, closing: ") + e.what());
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================

void
DatabaseHandler::buildTopology(const std::string& rTopoName,
							   int srid,
							   double tolerance)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}


		// TRANSACTION START
		pqxx::work transaction(mConnection);

		std::string temp_schema = TEMP_SCHEMA_PREFIX + transaction.esc(rTopoName);
		std::string temp_table = TEMP_TABLE_PREFIX + transaction.esc(rTopoName);

		installPostgisTopology(transaction);
		setSearchPath(transaction);
		createTemporaryTable(transaction, temp_table);
		createTemporarySchema(transaction, temp_schema, srid);
		addTopoGeometryColumn(transaction, temp_schema, temp_table);
		fillTopoGeometryColumn(transaction, temp_schema, temp_table, tolerance);

		// TRANSACTION END
		transaction.commit();
	}
	catch(const std::exception& e)
	{
		throw DatabaseException(std::string("Database error: ") + e.what());
	}
}


void
DatabaseHandler::removeTopology(const std::string& rTopoName)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}

		// TRANSACTION START
		pqxx::work transaction(mConnection);

		std::string temp_schema = TEMP_SCHEMA_PREFIX + transaction.esc(rTopoName);
		std::string temp_table = TEMP_TABLE_PREFIX + transaction.esc(rTopoName);

		dropTemporaryTable(transaction, temp_table);
		dropTemporarySchema(transaction, temp_schema);
		deleteTemporaryLayerRecord(transaction, temp_table);
		deleteTemporaryTopoRecord(transaction, temp_schema);

		// TRANSACTION END
		transaction.commit();
	}
	catch(const std::exception& e)
	{
		throw DatabaseException(std::string("Database error: ") + e.what());
	}

}


void
DatabaseHandler::getTopologyVertices(const std::string& rTopoName,
									 std::vector<TopologyVertex*>& rTopologyVertices)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		std::string temp_schema = TEMP_SCHEMA_PREFIX + non_trans.esc(rTopoName);
//		std::string temp_table = TEMP_TABLE_PREFIX + non_trans.esc(rTopoName);

		pqxx::result result = non_trans.exec(
				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
				"FROM " + temp_schema + ".node;"
		);

		for(int row = 0; row < result.size(); ++row) {
			Point p(result[row][1].as<double>(), result[row][2].as<double>());
			TopologyVertex* p_vertex = new TopologyVertex(result[row][0].as<int>(), p);
			rTopologyVertices.push_back(p_vertex);
		}
	}
	catch(const std::exception& e)
	{
		throw DatabaseException(std::string("Database error: ") + e.what());
	}
}


void
DatabaseHandler::getTopologyEdges(const std::string& rTopoName,
								  std::vector<TopologyEdge*>& rTopologyEdges)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction non_trans(mConnection);

		std::string temp_schema = TEMP_SCHEMA_PREFIX + non_trans.esc(rTopoName);

		pqxx::result result = non_trans.exec(
				"SELECT edge_id, start_node, end_node "
				"FROM " + temp_schema + ".edge_data "
				"ORDER BY edge_id ASC;"
		);

		for(int row = 0; row < result.size(); ++row) {
			Point p(result[row][1].as<double>(), result[row][2].as<double>());
			TopologyEdge* p_edge = new TopologyEdge(result[row][0].as<int>(),
					result[row][1].as<int>(),
					result[row][2].as<int>());
			rTopologyEdges.push_back(p_edge);
		}
	}
	catch(const std::exception& e)
	{
		throw DatabaseException(std::string("Database error: ") + e.what());
	}
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
DatabaseHandler::DatabaseHandler()
{}

void
DatabaseHandler::installPostgisTopology(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"CREATE EXTENSION IF NOT EXISTS postgis_topology"
	);
}

void
DatabaseHandler::setSearchPath(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"SET search_path = topology, public"
	);
}

void
DatabaseHandler::createTemporaryTable(pqxx::transaction_base& rTrans,
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
DatabaseHandler::createTemporarySchema(pqxx::transaction_base& rTrans,
									  const std::string& rSchemaName, int srid)
{
	rTrans.exec(
			"SELECT topology.CreateTopology('" +
			rSchemaName + "'," +
			rTrans.quote(srid) + ")"
	);
}

void
DatabaseHandler::addTopoGeometryColumn(pqxx::transaction_base& rTrans,
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
DatabaseHandler::fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
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
DatabaseHandler::dropTemporaryTable(pqxx::transaction_base& rTrans,
									const std::string& rTableName)
{
	rTrans.exec(
			"DROP TABLE IF EXISTS public." + rTableName
	);
}

void
DatabaseHandler::dropTemporarySchema(pqxx::transaction_base& rTrans,
									 const std::string& rSchemaName)
{
	rTrans.exec(
			"DROP SCHEMA IF EXISTS " + rSchemaName + " CASCADE"
	);
}

void
DatabaseHandler::deleteTemporaryLayerRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rTableName)
{
	rTrans.exec(
			"DELETE FROM topology.layer "
			"WHERE table_name = " + rTrans.quote(rTableName)
	);
}

void
DatabaseHandler::deleteTemporaryTopoRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rSchemaName)
{
	rTrans.exec(
			"DELETE FROM topology.topology "
			"WHERE name = " + rTrans.quote(rSchemaName)
	);
}

