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

		std::string temp_schema = "topo_" + transaction.esc(rTopoName);
		std::string temp_table = "highways_" + transaction.esc(rTopoName);

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
									  const std::string& rTablename)
{
	rTrans.exec(
			"CREATE TABLE public." + rTablename + " " +
			"AS SELECT * "
			"FROM planet_osm_line "
			"WHERE highway IS NOT NULL"
	);
}

void
DatabaseHandler::createTemporarySchema(pqxx::transaction_base& rTrans,
									  const std::string& rSchemaname, int srid)
{
	rTrans.exec(
			"SELECT topology.CreateTopology('" +
			rSchemaname + "'," +
			rTrans.quote(srid) + ")"
	);
}

void
DatabaseHandler::addTopoGeometryColumn(pqxx::transaction_base& rTrans,
									   const std::string& rSchemaname,
									   const std::string& rTablename)
{
	rTrans.exec(
			"SELECT topology.AddTopoGeometryColumn('" +
			rSchemaname + "', " +
			"'public', '" +
			rTablename + "', " +
			"'topo_geom', 'LINESTRING')"
	);
}

void
DatabaseHandler::fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
									    const std::string& rSchemaname,
									    const std::string& rTablename,
										double tolerance)
{
	rTrans.exec(
			"UPDATE public." +
			rTablename + " " +
			"SET topo_geom = topology.toTopoGeom(way, '" +
			rSchemaname +
			"', 1, " +
			rTrans.quote(tolerance) + ")"
	);
}
