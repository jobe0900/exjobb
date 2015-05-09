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
	: mDbConfig(rDatabaseConfig)
{
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
std::string
DatabaseHandler::getDatabaseVersion()
{
	try {
		pqxx::connection conn(mDbConfig.getConnectionString());
		if(!conn.is_open()) {
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}
		std::ostringstream oss;
		oss << conn.server_version();
		conn.disconnect();
		return oss.str();
	}
	catch(const std::exception& e) {
		throw DatabaseException(std::string("Database error: ") + e.what());
	}
}


void
DatabaseHandler::installPostgisTopology()
{
	try {
		pqxx::connection conn(mDbConfig.getConnectionString());
		if(!conn.is_open()) {
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}

		std::string sql = "CREATE EXTENSION IF NOT EXISTS postgis_topology";

		// Execute nontransaction query
		pqxx::nontransaction non_trans(conn);
		non_trans.exec(sql);

		conn.disconnect();
	}
	catch(const std::exception& e) {
		throw DatabaseException(std::string("Database error: ") + e.what());
	}
}


void
DatabaseHandler::buildTopology(const std::string& rTopoName, int srid)
{
	try
	{
		pqxx::connection conn(mDbConfig.getConnectionString());
		if(!conn.is_open())
		{
			throw DatabaseException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}


		// TRANSACTION START
		pqxx::work transaction(conn);

		std::string temp_schema = "topo_" + transaction.esc(rTopoName);
		std::string temp_table = "highways_" + transaction.esc(rTopoName);

		transaction.exec(
				"CREATE EXTENSION IF NOT EXISTS postgis_topology"
		);
		transaction.exec(
				"SET search_path = topology, public"
		);
		transaction.exec(
				"CREATE TABLE public." + temp_table + " " +
				"AS SELECT * "
				"FROM planet_osm_line "
				"WHERE highway IS NOT NULL"
		);
		transaction.exec(
				"SELECT topology.CreateTopology('" +
				temp_schema + "'," +
				transaction.quote(srid) + ")"
		);
		transaction.exec(
				"SELECT topology.AddTopoGeometryColumn('" +
				temp_schema + "', " +
				"'public', '" +
				temp_table + "', " +
				"'topo_geom', 'LINESTRING')"
		);
		transaction.exec(
				"UPDATE public." +
				temp_table + " " +
				"SET topo_geom = topology.toTopoGeom(way, '" +
				temp_schema +
				"', 1, 1.0)"
		);

		// TRANSACTION END
		transaction.commit();
		conn.disconnect();
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

