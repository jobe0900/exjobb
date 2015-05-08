/*
 * DatabaseHandler.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "PostgisOsmProvider.h"  // class implemented

#include <sstream>



/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
PostgisOsmProvider::PostgisOsmProvider(const DatabaseConfig& rDatabaseConfig)
	: mDbConfig(rDatabaseConfig)
{
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
std::string
PostgisOsmProvider::getDatabaseVersion()
{
	try {
		pqxx::connection conn(mDbConfig.getConnectionString());
		if(!conn.is_open()) {
			throw OsmProviderException(
					std::string("Could not open ") + mDbConfig.mDatabase);
		}
		std::ostringstream oss;
		oss << conn.server_version();
		conn.disconnect();
		return oss.str();
	}
	catch(const std::exception& e) {
		throw OsmProviderException(std::string("Database error: ") + e.what());
	}
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
PostgisOsmProvider::PostgisOsmProvider()
{}

