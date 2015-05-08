/*
 * DatabaseHandler.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

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

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
DatabaseHandler::DatabaseHandler()
{}

