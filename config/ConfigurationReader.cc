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
        fillSurfaceMaxSpeedCost(rCostConfig);
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

    CostConfig::DefaultSpeed::HighLowSpeed hilo;
    std::string type_string;
    OsmHighway::HighwayType type;

    for(size_t i = 0; i < OsmHighway::NR_HIGHWAY_TYPES; ++i)
    {
        type_string = OsmHighway::typeStrings().at(i);
        hilo.high = mPropertyTree.get<int>(prefix + type_string + ".high");
        hilo.low  = mPropertyTree.get<int>(prefix + type_string + ".low");
        type = static_cast<OsmHighway::HighwayType>(i);
        rCostConfig.defaultSpeed.addDefaultSpeed(type, hilo);
    }
}

void
ConfigurationReader::fillSurfaceMaxSpeedCost(CostConfig& rCostConfig) const
{
    std::string prefix("cost.surface.");

    Speed speed;
    std::string type_string;
    OsmHighway::SurfaceType type;

    for(size_t i = 0; i < OsmHighway::NR_SURFACE_TYPES; ++i)
    {
        type_string = OsmHighway::surfaceTypeStrings().at(i);
        speed = mPropertyTree.get<int>(prefix + type_string);
        type = static_cast<OsmHighway::SurfaceType>(i);
        rCostConfig.surfaceMaxSpeed.addSurfaceMaxSpeed(type, speed);
    }

}
