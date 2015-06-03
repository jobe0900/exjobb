/*
 * LineGraphUtility.cc
 *
 *  Created on: 2015-05-18
 *      Author: Jonas Bergman
 */

#include "LineGraphUtility.h"  // class implemented

#include "../mapprovider/postgis/PostGisProvider.h"
#include "../mapprovider/jsontest/JsonTestProvider.h"

#include <string>

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
LineGraphUtility::LineGraphUtility(const std::string& rFilename)
    : mrSettingsfile(rFilename),
      mConfig(),
      mpMapProvider(nullptr),
      mTopology(),
      mpGraph(nullptr)
{
    try
    {
        init();
    }
    catch (const std::exception& e)
    {
        throw LineGraphUtilityException(
            std::string("Error initializing LineGraphUtility: ") + e.what());
    }
}

LineGraphUtility::~LineGraphUtility()
{
    delete mpMapProvider;
    delete mpGraph;
}

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
LineGraphUtility::init()
{
    initConfiguration();
    initMapProvider();
    buildGraph();
}

void
LineGraphUtility::initConfiguration()
{
    try
    {
        ConfigurationReader config_reader(mrSettingsfile);
        config_reader.fillConfiguration(mConfig);
    }
    catch (ConfigurationException& ce)
    {
        throw LineGraphUtilityException(
            std::string("Error in Configuration: ") + ce.what());
    }
}

void
LineGraphUtility::initMapProvider()
{
    try
    {
        const TopologyConfig& r_topo_config = mConfig.getTopologyConfig();
        const std::string& r_provider_name = r_topo_config.providerName;

        if(r_provider_name == TopologyConfig::PROVIDER_POSTGIS)
        {
            mpMapProvider = new PostGisProvider(mConfig);
        }
//        else if(r_provider_name == TopologyConfig::PROVIDER_PGROUTING)
//        {
//            //TODO
//        }
        else if(r_provider_name == TopologyConfig::PROVIDER_JSONTEST)
        {
            mpMapProvider = new JsonTestProvider(mConfig);
        }
        else
        {
            throw MapProviderException("No valid MapProvider found");
        }
    }
    catch (MapProviderException& mpe)
    {
        delete mpMapProvider;

        throw LineGraphUtilityException(
            std::string("Error in MapProvider: ") + mpe.what());
    }
}

void
LineGraphUtility::initTopology()
{
    mpMapProvider->getTopology(mTopology);
}

void
LineGraphUtility::buildGraph()
{
    try
    {
        initTopology();
        mpGraph = new Graph(mTopology);
    }
    catch (const std::exception& e)
    {
        delete mpGraph;
        throw LineGraphUtilityException(
            std::string("Error when building Graph: ") + e.what());
    }
}



