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
//Graph::LineGraphType*
//LineGraphUtility::getLineGraph() const
//{
//    Graph::LineGraphType* p_lg = new Graph::LineGraphType(*(mpGraph->getBoostLineGraph()));
//    return p_lg;
//}

Graph::LineGraphType*
LineGraphUtility::getLineGraph()
{
    return new Graph::LineGraphType(*(mpGraph->getBoostLineGraph()));
//    return mpGraph->getBoostLineGraph();
}

//void
//LineGraphUtility::updateTopology()
//{
//    mTopology = Topology();
//    initTopology();
//    initRestrictionsAndCosts();
//    buildGraph();
//}
//
void
LineGraphUtility::updateRestrictionsAndCosts()
{
    mTopology.clearEdgeCostAndRestrictions();
    initRestrictionsAndCosts();
    buildGraph();
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
LineGraphUtility::init()
{
    initConfiguration();
    initMapProvider();
    initTopology();
    initRestrictionsAndCosts();
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
        delete mpMapProvider;
        delete mpGraph;
        throw LineGraphUtilityException(
            std::string("LineGraphUtility:initConfiguration: ") + ce.what());
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
        delete mpGraph;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:initMapProvider: ") + mpe.what());
    }
}

void
LineGraphUtility::initTopology()
{
    try
    {
        mpMapProvider->getTopology(mTopology);
    }
    catch (MapProviderException& mpe)
    {
        delete mpMapProvider;
        delete mpGraph;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:initTopology ") + mpe.what());
    }
}

void
LineGraphUtility::initRestrictionsAndCosts()
{
    try
    {
        mpMapProvider->setRestrictionsAndCosts(mTopology);
    }
    catch (MapProviderException& mpe)
    {
        delete mpMapProvider;
        delete mpGraph;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:initRestrictionsAndCosts ") + mpe.what());
    }
}

void
LineGraphUtility::buildGraph()
{
    try
    {
        delete mpGraph;
        mpGraph = new Graph(mTopology, mConfig);
    }
    catch (const std::exception& e)
    {
        delete mpMapProvider;
        delete mpGraph;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:buildGraph: ") + e.what());
    }
}



