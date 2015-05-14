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
	catch (boost::property_tree::json_parser_error& e)
	{
		throw ConfigurationException("Could not read file " + mFilename);
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
const Configuration*
ConfigurationReader::getConfiguration() const
{
    Configuration* p_config = new Configuration();
    try
    {
        p_config->setDatabaseConfig(this->getDatabaseConfiguration());
        p_config->setVehicleConfig(this->getVehicleConfiguration());

        return p_config;
    }
    catch (ConfigurationException& e)
    {
        delete p_config;
        throw e;
    }
}




//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const DatabaseConfig*
ConfigurationReader::getDatabaseConfiguration() const
{
    DatabaseConfig* p_db_config = new DatabaseConfig();
    std::string prefix("database.");

    try
    {
        p_db_config->hostname = mPropertyTree.get<std::string>(prefix + "host");
        p_db_config->port = mPropertyTree.get<int>(prefix + "port");
        p_db_config->username = mPropertyTree.get<std::string>(prefix + "username");
        p_db_config->password = mPropertyTree.get<std::string>(prefix + "password");
        p_db_config->database = mPropertyTree.get<std::string>(prefix + "database");
        return p_db_config;
    }
    catch (boost::property_tree::ptree_error& e)
    {
        delete p_db_config;
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}


const VehicleConfig*
ConfigurationReader::getVehicleConfiguration() const
{
    VehicleConfig* p_v_config = new VehicleConfig();
    std::string prefix("vehicle.");

    try
    {
        p_v_config->category = mPropertyTree.get<std::string>(prefix + "category");
        p_v_config->height = mPropertyTree.get<double>(prefix + "height");
        p_v_config->length = mPropertyTree.get<double>(prefix + "length");
        p_v_config->weight = mPropertyTree.get<double>(prefix + "weight");
        p_v_config->width = mPropertyTree.get<double>(prefix + "width");
        return p_v_config;
    }
    catch (boost::property_tree::ptree_error& e)
    {
        delete p_v_config;
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}
