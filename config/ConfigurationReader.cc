/*
 * ConfigurationReader.cc
 *
 *  Created on: 7 maj 2015
 *      Author: jonas
 */

#include "ConfigurationReader.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================

ConfigurationReader::ConfigurationReader(const std::string& rFilename)
	: mFilename(rFilename)
{
	boost::property_tree::read_json(mFilename, mPropertyTree);
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================

void
ConfigurationReader::getDatabaseConfiguration(DatabaseConfig& rDatabaseConfig)
{
	std::string prefix("database.");

	rDatabaseConfig.mHostname = mPropertyTree.get<std::string>(prefix + "host");
	rDatabaseConfig.mPort = mPropertyTree.get<int>(prefix + "port");
	rDatabaseConfig.mUsername = mPropertyTree.get<std::string>(prefix + "username");
	rDatabaseConfig.mPassword = mPropertyTree.get<std::string>(prefix + "password");
	rDatabaseConfig.mDatabase = mPropertyTree.get<std::string>(prefix + "database");
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
ConfigurationReader::ConfigurationReader()
{}

