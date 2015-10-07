/*
 * LineGraphUtility.cc
 *
 *      Author: Jonas Bergman
 */

#include "../mapprovider/postgis/PostGisProvider.h"
#include "../mapprovider/jsontest/JsonTestProvider.h"

#include <string>
#include "LineGraphUtility.h"  // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
LineGraphUtility::LineGraphUtility(const std::string& rFilename)
    : mrSettingsfile(rFilename),
      mConfig(),
      mpMapProvider(nullptr),
      mTopology(),
      mpGraphBuilder(nullptr)
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
    delete mpGraphBuilder;
}

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
LineGraphType*
LineGraphUtility::getLineGraph()
{
    LineGraphType& r_orig = mpGraphBuilder->getBoostLineGraph();
    LineGraphType* p_new = new LineGraphType();

    // make a copy of the old graph into a new
    boost::copy_graph(r_orig, *p_new);

    return p_new;
}

void
LineGraphUtility::updateTopology()
{
    mTopology.clearTopology();
    initTopology();
    initRestrictionsAndCosts();
    buildGraph();
}

void
LineGraphUtility::updateRestrictionsAndCosts()
{
    mTopology.clearEdgeCostAndRestrictions();
    initRestrictionsAndCosts();
    buildGraph();
}

void
LineGraphUtility::persistLineGraph()
{
    try
    {
        mpMapProvider->persistLineGraph(*mpGraphBuilder);
    }
    catch(MapProviderException& mpe)
    {
        throw LineGraphUtilityException(mpe.what());
    }
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
        delete mpGraphBuilder;
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
        delete mpGraphBuilder;

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
        delete mpGraphBuilder;

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
        delete mpGraphBuilder;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:initRestrictionsAndCosts ") + mpe.what());
    }
}

void
LineGraphUtility::buildGraph()
{
    try
    {
        delete mpGraphBuilder;
        mpGraphBuilder = new GraphBuilder(mTopology, mConfig);
    }
    catch (const std::exception& e)
    {
        delete mpMapProvider;
        delete mpGraphBuilder;

        throw LineGraphUtilityException(
            std::string("LineGraphUtility:buildGraph: ") + e.what());
    }
}
