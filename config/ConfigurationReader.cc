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
	try
	{
		boost::property_tree::read_json(mFilename, mPropertyTree);
	}
	catch (boost::property_tree::json_parser_error& e) {
		throw ConfigurationException("Could not read file " + mFilename);
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================

void
ConfigurationReader::getDatabaseConfiguration(DatabaseConfig& rDatabaseConfig)
{
	std::string prefix("database.");

	try
	{
		rDatabaseConfig.mHostname = mPropertyTree.get<std::string>(prefix + "host");
		rDatabaseConfig.mPort = mPropertyTree.get<int>(prefix + "port");
		rDatabaseConfig.mUsername = mPropertyTree.get<std::string>(prefix + "username");
		rDatabaseConfig.mPassword = mPropertyTree.get<std::string>(prefix + "password");
		rDatabaseConfig.mDatabase = mPropertyTree.get<std::string>(prefix + "database");
	}
	catch (boost::property_tree::ptree_error& e)
	{
		throw ConfigurationException(std::string("Could not read config ") + e.what());
	}
}


void
ConfigurationReader::getVehicleConfiguration(VehicleConfig& rVehicleConfig)
{
	std::string prefix("vehicle.");

	try
	{
		rVehicleConfig.mCategory = mPropertyTree.get<std::string>(prefix + "category");
		rVehicleConfig.mHeight = mPropertyTree.get<double>(prefix + "height");
		rVehicleConfig.mLength = mPropertyTree.get<double>(prefix + "length");
		rVehicleConfig.mWeight = mPropertyTree.get<double>(prefix + "weight");
		rVehicleConfig.mWidth = mPropertyTree.get<double>(prefix + "width");
	}
	catch (boost::property_tree::ptree_error& e)
	{
		throw ConfigurationException(std::string("Could not read config ") + e.what());
	}
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
ConfigurationReader::ConfigurationReader()
{}

