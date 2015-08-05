/*
 * ConfigurationReader.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
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
    fillAccessRule(rConfig.mAccessRule);
    fillBarrierRestrictRule(rConfig.mBarrierRestrictionsRule);
    fillBarrierCostsRule(rConfig.mBarrierCostsRule);
    fillCostConfiguration(rConfig.mCostConfig);
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

            rTopoConfig.topoName =
                mPropertyTree.get<std::string>(prefix + "topo_name");

            rTopoConfig.roadsPrefix =
                mPropertyTree.get<std::string>(prefix + "roads_prefix");
            rTopoConfig.topologySchemaPrefix =
                mPropertyTree.get<std::string>(prefix + "schema_prefix");

            rTopoConfig.tempTopoName =
                mPropertyTree.get<std::string>(prefix + "build.temp_topo_name");
            rTopoConfig.srid =
                mPropertyTree.get<int>(prefix + "build.srid");
            rTopoConfig.tolerance =
                mPropertyTree.get<double>(prefix + "build.tolerance");

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
        std::string categoryString =
            mPropertyTree.get<std::string>(prefix + "category");
        rVehicleConfig.category = OsmVehicle::parseString(categoryString);
        prefix += categoryString + ".";
        rVehicleConfig.height = mPropertyTree.get<double>(prefix + "height");
        rVehicleConfig.length = mPropertyTree.get<double>(prefix + "length");
        rVehicleConfig.weight = mPropertyTree.get<double>(prefix + "weight");
        rVehicleConfig.width = mPropertyTree.get<double>(prefix + "width");
        rVehicleConfig.maxspeed = mPropertyTree.get<double>(prefix + "maxspeed");
    }
    catch (ConfigurationException& e)
    {
        throw e;
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}


void
ConfigurationReader::fillAccessRule(OsmAccess::AccessRule& rAccessRule) const
{
    std::string prefix("access.allow");

    try
    {
        std::vector<OsmAccess::AccessType> allow_tags;
        for(auto& item : mPropertyTree.get_child(prefix))
        {
            std::string tag_string = item.second.get_value<std::string>();
            allow_tags.push_back(OsmAccess::parseString(tag_string));
        }
        rAccessRule.allowAccessToTypes = allow_tags;
    }
    catch (ConfigurationException& e)
    {
        throw e;
    }
    catch (OsmException& ose)
    {
        throw ConfigurationException(std::string("Could not read config") +
            ", error parsing access tag: " + ose.what());
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}

void
ConfigurationReader::fillBarrierRestrictRule(OsmBarrier::RestrictionsRule& rRestrictRule) const
{
    std::string prefix("restrict.barriers");

    try
    {
        std::vector<OsmBarrier::BarrierType> restrict_barriers;
        for(auto& item : mPropertyTree.get_child(prefix))
        {
            std::string restrict_string = item.second.get_value<std::string>();
            restrict_barriers.push_back(OsmBarrier::parseString(restrict_string));
        }
        rRestrictRule.restrictionTypes = restrict_barriers;
    }
    catch (ConfigurationException& e)
    {
        throw e;
    }
    catch (OsmException& ose)
    {
        throw ConfigurationException(std::string("Could not read config") +
            ", error parsing barrier restrictions: " + ose.what());
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}

void
ConfigurationReader::fillBarrierCostsRule(OsmBarrier::CostsRule& rCostsRule) const
{
    std::string prefix("cost.barriers");

    try
    {
        for(auto& row : mPropertyTree.get_child(prefix))
        {
            int i = 0;
            std::string type_string;
            unsigned cost;
            for(auto& item : row.second)
            {
                if(i == 0)
                {
                    type_string = item.second.get_value<std::string>();
                }
                else
                {
                    cost = item.second.get_value<unsigned>();
                }
                ++i;
            }
            OsmBarrier::BarrierType barrier_type = OsmBarrier::parseString(type_string);
            rCostsRule.addCost(barrier_type, cost);
        }
    }
    catch (ConfigurationException& e)
    {
        throw e;
    }
    catch (OsmException& ose)
    {
        throw ConfigurationException(std::string("Could not read config") +
            ", error parsing barrier costs: " + ose.what());
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}

void
ConfigurationReader::fillCostConfiguration(CostConfig& rCostConfig) const
{
    try
    {
        fillDefaultSpeedCost(rCostConfig);
    }
    catch (ConfigurationException& e)
    {
        throw e;
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw ConfigurationException(std::string("Could not read config ") + e.what());
    }
}

void
ConfigurationReader::fillDefaultSpeedCost(CostConfig& rCostConfig) const
{
    std::string prefix("cost.default_speed.");

    rCostConfig.defaultSpeed.motorway.high =
        mPropertyTree.get<int>(prefix + "motorway.high");
    rCostConfig.defaultSpeed.motorway.low =
        mPropertyTree.get<int>(prefix + "motorway.low");
    rCostConfig.defaultSpeed.motorway_link.high =
        mPropertyTree.get<int>(prefix + "motorway_link.high");
    rCostConfig.defaultSpeed.motorway_link.low =
        mPropertyTree.get<int>(prefix + "motorway_link.low");

    rCostConfig.defaultSpeed.trunk.high =
        mPropertyTree.get<int>(prefix + "trunk.high");
    rCostConfig.defaultSpeed.trunk.low =
        mPropertyTree.get<int>(prefix + "trunk.low");
    rCostConfig.defaultSpeed.trunk_link.high =
        mPropertyTree.get<int>(prefix + "trunk_link.high");
    rCostConfig.defaultSpeed.trunk_link.low =
        mPropertyTree.get<int>(prefix + "trunk_link.low");

    rCostConfig.defaultSpeed.primary.high =
        mPropertyTree.get<int>(prefix + "primary.high");
    rCostConfig.defaultSpeed.primary.low =
        mPropertyTree.get<int>(prefix + "primary.low");
    rCostConfig.defaultSpeed.primary_link.high =
        mPropertyTree.get<int>(prefix + "primary_link.high");
    rCostConfig.defaultSpeed.primary_link.low =
        mPropertyTree.get<int>(prefix + "primary_link.low");

    rCostConfig.defaultSpeed.secondary.high =
        mPropertyTree.get<int>(prefix + "secondary.high");
    rCostConfig.defaultSpeed.secondary.low =
        mPropertyTree.get<int>(prefix + "secondary.low");
    rCostConfig.defaultSpeed.secondary_link.high =
        mPropertyTree.get<int>(prefix + "secondary_link.high");
    rCostConfig.defaultSpeed.secondary_link.low =
        mPropertyTree.get<int>(prefix + "secondary_link.low");

    rCostConfig.defaultSpeed.tertiary.high =
        mPropertyTree.get<int>(prefix + "tertiary.high");
    rCostConfig.defaultSpeed.tertiary.low =
        mPropertyTree.get<int>(prefix + "tertiary.low");
    rCostConfig.defaultSpeed.tertiary_link.high =
        mPropertyTree.get<int>(prefix + "tertiary_link.high");
    rCostConfig.defaultSpeed.tertiary_link.low =
        mPropertyTree.get<int>(prefix + "tertiary_link.low");

    rCostConfig.defaultSpeed.unclassified.high =
        mPropertyTree.get<int>(prefix + "unclassified.high");
    rCostConfig.defaultSpeed.unclassified.low =
        mPropertyTree.get<int>(prefix + "unclassified.low");

    rCostConfig.defaultSpeed.residential.high =
        mPropertyTree.get<int>(prefix + "residential.high");
    rCostConfig.defaultSpeed.residential.low =
        mPropertyTree.get<int>(prefix + "residential.low");

    rCostConfig.defaultSpeed.living_street.high =
        mPropertyTree.get<int>(prefix + "living_street.high");
    rCostConfig.defaultSpeed.living_street.low =
        mPropertyTree.get<int>(prefix + "living_street.low");
}
