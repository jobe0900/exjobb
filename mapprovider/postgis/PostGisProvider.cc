/*
 * PostGisProvider.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "PostGisProvider.h"  // class implemented
#include "TopologyQueries.h"
#include "RestrictionQueries.h"
#include "CostQueries.h"

#include "../../graph/Edge.h"
#include "../../osm/OsmId.h"
#include "../../graph/EdgeRestriction.h"
#include "../../graph/EdgeCost.h"



/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
PostGisProvider::PostGisProvider(const Configuration& rConfig)
try
    : MapProvider(rConfig),
      mConfig(rConfig),
      mDbConfig(rConfig.getDatabaseConfig()),
      mTopoConfig(rConfig.getTopologyConfig()),
      mConnection(mDbConfig.getConnectionString())
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        std::string topoBaseName;
        setTopoBaseName(topoBaseName);

        if(topoBaseName == "")
        {
            throw MapProviderException("No topology specified.");
        }

        pqxx::nontransaction nt(mConnection);
        mOsmEdgeTable       = nt.esc(mTopoConfig.roadsPrefix + "_" +
                                topoBaseName);
        mPointTableName     = nt.esc("planet_osm_point");
        mSchemaName         = nt.esc(mTopoConfig.topologySchemaPrefix + "_" +
                                topoBaseName);
        mTopoEdgeTable      = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeTableName);
        mEdgeIdCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeIdColumnName);
        mSourceCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.sourceColumnName);
        mTargetCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.targetColumnName);
        mEdgeGeomCol        = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeGeomColumnName);
        mTopoVertexTable    = nt.esc(mSchemaName + "." +
                                mTopoConfig.vertexTableName);
        mVertexIdCol        = nt.esc(mSchemaName + "." +
                                mTopoConfig.vertexIdColumnName);
        mVertexGeomCol      = nt.esc(mSchemaName + "." +
                                mTopoConfig.vertexGeomColumnName);
        nt.abort();

        if(mBuildTempTopology)
        {
            buildTopology(mTopoConfig.srid, mTopoConfig.tolerance);
        }
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:ctor(in): ") + e.what());
    }
}
// catch error in initializer list (opening connection)
catch(const std::exception& e)
{
        throw MapProviderException(
            std::string("PostGisProvider:ctor(out): ") + e.what());
}

PostGisProvider::~PostGisProvider()
{
	try
	{
	    if(mBuildTempTopology)
	    {
	        removeTopology();
	    }
		if(mConnection.is_open())
		{
			mConnection.disconnect();
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:dtor: ") + e.what());
	}
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
void
PostGisProvider::getTopology(Topology& rTopology)
{
    pqxx::result vertex_result;
    getTopologyVertices(vertex_result);
    addVertexResultToTopology(vertex_result, rTopology);

    pqxx::result edge_result;
    getTopologyEdges(edge_result);
    addEdgeResultToTopology(edge_result, rTopology);
}

void
PostGisProvider::setRestrictionsAndCosts(Topology& rTopology)
{
    addEdgeRestrictions(rTopology);
    addEdgeCosts(rTopology);
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////
void
PostGisProvider::getTopologyVertices(pqxx::result& rVertexResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction transaction(mConnection);

		TopologyQueries::getTopologyVertices(
		    transaction,
		    rVertexResult,
		    mTopoVertexTable);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopologyVertices: ") + e.what());
	}
}

void
PostGisProvider::addVertexResultToTopology(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    TopologyQueries::addVertexResultToTopology(rResult, rTopology);
}


void
PostGisProvider::getTopologyEdges(pqxx::result& rEdgeResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction transaction(mConnection);
		TopologyQueries::getTopologyEdges(
		    transaction,
		    rEdgeResult,
		    mTopoEdgeTable,
		    mSchemaName,
		    mOsmEdgeTable);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopoEdges: ") + e.what());
	}
}

void
PostGisProvider::addEdgeResultToTopology(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    TopologyQueries::addEdgeResultToTopology(rResult, rTopology);
}


void
PostGisProvider::buildTopology(int srid, double tolerance)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// TRANSACTION START
		pqxx::work transaction(mConnection);

		try
		{
		    TopologyQueries::installPostgisTopology(transaction);
		    TopologyQueries::setSearchPath(transaction);
		    TopologyQueries::createTemporaryTable(transaction, mOsmEdgeTable);
		    TopologyQueries::createTemporarySchema(
		        transaction, mSchemaName, srid);
		    TopologyQueries::addTopoGeometryColumn(
		        transaction, mSchemaName, mOsmEdgeTable);
		    TopologyQueries::fillTopoGeometryColumn(
		        transaction, mSchemaName, mOsmEdgeTable, tolerance);

		    // TRANSACTION END
		    transaction.commit();
		}
		catch (const std::exception& e)
		{
		    transaction.abort();
		    throw e;
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:buildTopology: ") + e.what());
	}
}


void
PostGisProvider::removeTopology()
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// TRANSACTION START
		pqxx::work transaction(mConnection);

		try
		{
		    TopologyQueries::dropTemporaryTable(transaction, mOsmEdgeTable);
		    TopologyQueries::dropTemporarySchema(transaction, mSchemaName);
		    TopologyQueries::deleteTemporaryLayerRecord(transaction, mOsmEdgeTable);
		    TopologyQueries::deleteTemporaryTopoRecord(transaction, mSchemaName);

		    // TRANSACTION END
		    transaction.commit();
		}
		catch (const std::exception& e)
		{
		    transaction.abort();
		    throw e;
		}
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(std::string(
            "PostGisProvider:removeTopology: ") + e.what());
	}
}

void
PostGisProvider::setTopoBaseName(std::string& rTopoBaseName)
{
    if(mTopoConfig.tempTopoName == TopologyConfig::TEMP_TOPO_NAMEBASE)
    {
        rTopoBaseName = TimeToStringMaker::getEpochMsTimeString();
        mBuildTempTopology = true;
    }
    else
    {
        rTopoBaseName = mTopoConfig.topoName;
    }
}

// Restrictions --------------------------------------------------------------
void
PostGisProvider::addEdgeRestrictions(Topology& rTopology)
{
    pqxx::result result;

    getVehiclePropertyEdgeRestrictions(result);
    addVehiclePropertyRestrictionsToEdge(result, rTopology);

    result.clear();
    getAccessRestrictions(result);
    addAccessRestrictionsToEdge(result, rTopology);

    result.clear();
    getTurningRestrictions(result);
    addTurningRestrictionsToEdge(result, rTopology);

    result.clear();
    getEdgePointRestrictions(result);
    addPointRestrictionsToEdge(result, rTopology);
}

void
PostGisProvider::getVehiclePropertyEdgeRestrictions(pqxx::result& rResult)
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        // NON-TRANSACTION START
        pqxx::nontransaction transaction(mConnection);

        RestrictionQueries::getVehiclePropertyEdgeRestrictions(
            transaction,
            rResult,
            mTopoEdgeTable,
            mOsmEdgeTable,
            mSchemaName
        );
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:getVehiclePropertyEdgeRestrictions: ")
                        + e.what());
    }
}

void
PostGisProvider::addVehiclePropertyRestrictionsToEdge(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    RestrictionQueries::addVehiclePropertyRestrictionsToEdge(rResult, rTopology);
}

void
PostGisProvider::getAccessRestrictions(pqxx::result& rResult)
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        // NON-TRANSACTION START
        pqxx::nontransaction transaction(mConnection);

        RestrictionQueries::getAccessRestrictions(
            transaction,
            rResult,
            mTopoEdgeTable,
            mOsmEdgeTable,
            mSchemaName);
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:getAccessRestrictions: ")
        + e.what());
    }
}

void
PostGisProvider::addAccessRestrictionsToEdge(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    RestrictionQueries::addAccessRestrictionsToEdge(rResult, rTopology, mConfig);
}

void
PostGisProvider::getTurningRestrictions(pqxx::result& rResult)
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        // TRANSACTION START
        pqxx::nontransaction transaction(mConnection);

        try
        {
            RestrictionQueries::dropCreateTurningRestrictionsTable(transaction);
            RestrictionQueries::identifyTurningRestrictions(
                transaction,
                mOsmEdgeTable,
                mTopoEdgeTable);
            RestrictionQueries::getTurningRestrictions(transaction, rResult);
        }
        catch (const std::exception& e)
        {
            transaction.abort();
            throw e;
        }
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:getTurningRestrictions: ")
        + e.what());
    }
}

void
PostGisProvider::addTurningRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    RestrictionQueries::addTurningRestrictionsToEdge(rResult, rTopology);
}

void
PostGisProvider::getEdgePointRestrictions(pqxx::result& rResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction transaction(mConnection);

		RestrictionQueries::getEdgePointRestrictions(
		    transaction,
		    rResult,
		    mPointTableName,
		    mTopoEdgeTable,
		    mOsmEdgeTable,
		    mSchemaName);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getEdgePointRestrictions: ") + e.what());
	}
}

void
PostGisProvider::addPointRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    RestrictionQueries::addPointRestrictionsToEdge(rResult, rTopology, mConfig);
}

// Costs ---------------------------------------------------------------------
void
PostGisProvider::addEdgeCosts(Topology& rTopology)
{
    pqxx::result result;

    getTravelTimeCosts(result);
    addTravelTimeCosts(result, rTopology);

    // barrier costs are added while looking for restrictions

    result.clear();
    getOtherEdgeCosts(result);
    addOtherCosts(result, rTopology);
}

void
PostGisProvider::getTravelTimeCosts(pqxx::result& rResult)
{
    try
    {
        if(!mConnection.is_open())
        {
            throw MapProviderException(
                std::string("Could not open ") + mDbConfig.database);
        }

        // NON-TRANSACTION START
        pqxx::nontransaction transaction(mConnection);

        CostQueries::getTravelTimeEdgeCosts(
            transaction,
            rResult,
            mTopoEdgeTable,
            mOsmEdgeTable,
            mSchemaName
        );
    }
    catch(const std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:getTravelTimCost: ")
                        + e.what());
    }
}

void
PostGisProvider::addTravelTimeCosts(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    CostQueries::addTravelTimeCosts(rResult, rTopology, mConfig);
}

void
PostGisProvider::getOtherEdgeCosts(pqxx::result& rResult)
{
	try
	{
		if(!mConnection.is_open())
		{
			throw MapProviderException(
					std::string("Could not open ") + mDbConfig.database);
		}

		// NON-TRANSACTION START
		pqxx::nontransaction transaction(mConnection);

		CostQueries::getOtherCosts(
		    transaction,
		    rResult,
		    mPointTableName,
		    mTopoEdgeTable,
		    mOsmEdgeTable,
		    mSchemaName);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getOtherEdgeCosts: ") + e.what());
	}
}

void
PostGisProvider::addOtherCosts(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    CostQueries::addOtherCosts(rResult, rTopology, mConfig);
}
