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
void
ConfigurationReader::fillConfiguration(Configuration& rConfig) const
{
    fillDatabaseConfiguration(rConfig.mDbConfig);
    fillTopologyConfiguration(rConfig.mTopoConfig);
    fillVehicleConfiguration(rConfig.mVehicleConfig);
}


//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
ConfigurationReader::fillDatabaseConfiguration(DatabaseConfig& rDbConfig) const
{
    std::string prefix("database.");

    try
    {
        rDbConfig.hostname = mPropertyTree.get<std::string>(prefix + "host");
        rDbConfig.port = mPropertyTree.get<int>(prefix + "port");
        rDbConfig.username = mPropertyTree.get<std::string>(prefix + "username");
        rDbConfig.password = mPropertyTree.get<std::string>(prefix + "password");
        rDbConfig.database = mPropertyTree.get<std::string>(prefix + "database");
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}

void
ConfigurationReader::fillTopologyConfiguration(TopologyConfig& rTopoConfig) const
{
    std::string prefix("topology.");

    try
    {
        rTopoConfig.providerName =
            mPropertyTree.get<std::string>(prefix + "provider");

        if(rTopoConfig.providerName == TopologyConfig::PROVIDER_JSONTEST)
        {
            rTopoConfig.testFile =
                mPropertyTree.get<std::string>(prefix + "jsontest.test_file");
        }
        else if(rTopoConfig.providerName == TopologyConfig::PROVIDER_POSTGIS
             || rTopoConfig.providerName == TopologyConfig::PROVIDER_PGROUTING)
        {
            prefix += rTopoConfig.providerName + ".";
            rTopoConfig.tempTopoName =
                mPropertyTree.get<std::string>(prefix + "temp_topo");
            rTopoConfig.topoName =
                mPropertyTree.get<std::string>(prefix + "topo_name");

            rTopoConfig.roadsPrefix =
                mPropertyTree.get<std::string>(prefix + "roads_prefix");
            rTopoConfig.topologySchemaPrefix =
                mPropertyTree.get<std::string>(prefix + "schema_prefix");

            rTopoConfig.edgeTableName =
                mPropertyTree.get<std::string>(prefix + "edge.table");
            rTopoConfig.edgeIdColumnName =
                mPropertyTree.get<std::string>(prefix + "edge.id_col");
            rTopoConfig.sourceColumnName =
                mPropertyTree.get<std::string>(prefix + "edge.source_col");
            rTopoConfig.targetColumnName=
                mPropertyTree.get<std::string>(prefix + "edge.target_col");
            rTopoConfig.edgeGeomColumnName =
                mPropertyTree.get<std::string>(prefix + "edge.geom_col");

            rTopoConfig.vertexTableName =
                mPropertyTree.get<std::string>(prefix + "vertex.table");
            rTopoConfig.vertexIdColumnName =
                mPropertyTree.get<std::string>(prefix + "vertex.id_col");
            rTopoConfig.vertexGeomColumnName =
                mPropertyTree.get<std::string>(prefix + "vertex.geom_col");
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}


void
ConfigurationReader::fillVehicleConfiguration(VehicleConfig& rVehicleConfig) const
{
    std::string prefix("vehicle.");

    try
    {
        rVehicleConfig.category = mPropertyTree.get<std::string>(prefix + "category");
        prefix += rVehicleConfig.category + ".";
        rVehicleConfig.height = mPropertyTree.get<double>(prefix + "height");
        rVehicleConfig.length = mPropertyTree.get<double>(prefix + "length");
        rVehicleConfig.weight = mPropertyTree.get<double>(prefix + "weight");
        rVehicleConfig.width = mPropertyTree.get<double>(prefix + "width");
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}
