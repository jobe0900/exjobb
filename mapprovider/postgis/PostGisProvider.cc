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
//    try
//    {
//        for(const pqxx::tuple& row : rResult)
//        {
//            // throw exception if no edgeId
//            EdgeIdType edgeId =
//                row[RestrictionQueries::VehiclePropertiesRestrictions::EDGE_ID]
//                    .as<EdgeIdType>();
//
//            Edge& edge = rTopology.getEdge(edgeId);
//            EdgeRestriction& r_restrictions = edge.restrictions();
//
//            EdgeRestriction::VehicleProperties* p_vp =
//                new EdgeRestriction::VehicleProperties();
//
//            p_vp->maxHeight =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MAXHEIGHT].as<double>
//                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            p_vp->maxLength =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MAXLENGTH].as<double>
//                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            p_vp->maxWeight =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MAXWEIGHT].as<double>
//                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            p_vp->maxWidth =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MAXWIDTH].as<double>
//                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            p_vp->maxSpeed =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MAXSPEED].as<unsigned>
//                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
//            p_vp->minSpeed =
//                row[RestrictionQueries::
//                    VehiclePropertiesRestrictions::MINSPEED].as<unsigned>
//                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MIN);
//
//            r_restrictions.setVehiclePropertyRestriction(p_vp);
//
//        }
//    }
//    catch (std::exception& e)
//    {
//        throw MapProviderException(
//            std::string("PostGisProvider:addVehicleProp..ToEdge..: ") + e.what());
//    }
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
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[RestrictionQueries::
                    AccessRestrictions::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            std::string colString;
            colString = row[RestrictionQueries::AccessRestrictions::ACCESS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.setGeneralAccessRestriction(type);
            }

            colString = row[RestrictionQueries::AccessRestrictions::MOTORCAR]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTORCAR,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::GOODS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::GOODS,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::HGV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::HGV,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::PSV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::PSV,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::LHV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::LHV,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::MOTOR_VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTOR_VEHICLE,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::VEHICLE,
                    type
                );
            }

            colString = row[RestrictionQueries::AccessRestrictions::BARRIER]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmBarrier::BarrierType type = OsmBarrier::parseString(colString);
                r_restrictions.setBarrierRestriction(type);
                addBarrierCostToEdge(edge, type);
            }

            colString = row[RestrictionQueries::AccessRestrictions::DISUSED]
                            .as<std::string>("");
            if(colString == "yes")
            {
                r_restrictions.setDisusedRestriction();
            }

            colString = row[RestrictionQueries::AccessRestrictions::NOEXIT]
                            .as<std::string>("");
            if(colString == "yes")
            {
                r_restrictions.setNoExitRestriction();
            }
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addAccessResultToEdge..: ") + e.what());
    }
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
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            OsmTurningRestriction* p_turn =
                RestrictionQueries::TurningRestrictions::Results::
                    parseTurningRestrictionResultRow(row, rTopology);

            // mark edge as having a restriction
            Edge& edge = rTopology.getEdge(p_turn->fromEdgeId());
            EdgeRestriction& r_restrictions = edge.restrictions();
            r_restrictions.addTurningRestriction(p_turn);

            // explicit mark "VIA WAY"
            if(p_turn->viaType() == OsmTurningRestriction::VIA_WAY)
            {
                r_restrictions.setViaWayRestriction();
            }
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addTurningResultToEdge..: ") + e.what());
    }
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
		    mTopoEdgeTable);
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
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[RestrictionQueries::EdgePointRestrictions::EDGE_ID]
                    .as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            std::string barrierTypeString =
                row[RestrictionQueries::EdgePointRestrictions::BARRIER]
                    .as<std::string>();
            OsmBarrier::BarrierType barrierType =
                OsmBarrier::parseString(barrierTypeString);
            r_restrictions.setBarrierRestriction(barrierType);
            addBarrierCostToEdge(edge, barrierType);

            std::string colString;
            colString = row[RestrictionQueries::EdgePointRestrictions::ACCESS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.setGeneralAccessRestriction(type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::GOODS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::GOODS,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::HGV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::HGV,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::LHV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::LHV,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTORCAR]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTORCAR,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTOR_VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTOR_VEHICLE,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::PSV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::PSV,
                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::VEHICLE,
                    type);
            }
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addPointResultToEdge..: ") + e.what());
    }
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
            std::string("PostGisProvider:getVehiclePropertyEdgeRestrictions: ")
                        + e.what());
    }
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
		    mTopoEdgeTable);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getOtherEdgeCosts: ") + e.what());
	}
}

void
PostGisProvider::addTravelTimeCosts(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[CostQueries::TravelTimeCostResult::EDGE_ID]
                    .as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);

            Speed speed =
                row[CostQueries::TravelTimeCostResult::MAXSPEED].as<Speed>(
                    EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
            std::string surface_string =
                row[CostQueries::TravelTimeCostResult::SURFACE].as<std::string>("");

            addTravelTimeCostToEdge(edge, speed, surface_string);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addTravelTimeCost: ") + e.what());
    }
}

void
PostGisProvider::addOtherCosts(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[CostQueries::OtherCostResult::EDGE_ID]
                    .as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);

            std::string type_string = "highway=" +
                row[CostQueries::OtherCostResult::HIGHWAY]
                    .as<std::string>("");
            addOtherCostToEdge(edge, type_string);

            type_string = "railway=" +
                row[CostQueries::OtherCostResult::RAILWAY]
                    .as<std::string>("");
            addOtherCostToEdge(edge, type_string);

            type_string = "public_transport=" +
                row[CostQueries::OtherCostResult::PUBLIC_TRANSPORT]
                    .as<std::string>("");
            addOtherCostToEdge(edge, type_string);

            type_string = "traffic_calming=" +
                row[CostQueries::OtherCostResult::TRAFFIC_CALMING]
                    .as<std::string>("");
            addOtherCostToEdge(edge, type_string);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addPointResultToEdge..: ") + e.what());
    }
}

void
PostGisProvider::addBarrierCostToEdge(Edge& rEdge, OsmBarrier::BarrierType type)
{
    if(mConfig.getBarrierCostsRule().costsToPass(type))
    {
        Cost cost = mConfig.getBarrierCostsRule().getCost(type);
        rEdge.edgeCost().addCost(EdgeCost::BARRIER, cost);
    }
}


void
PostGisProvider::addTravelTimeCostToEdge(
    Edge&           rEdge,
    Speed           speed,
    std::string&    surfaceString)
{
    bool hasMaxSpeed =
        (speed != EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
    bool hasSurface = surfaceString.length() > 0;
    if(!(hasMaxSpeed || hasSurface))
    {
        speed = getDefaultSpeedForEdge(rEdge);
    }
    // look if surface restricts speed
    else if(hasSurface)
    {
        try
        {
            OsmHighway::SurfaceType surface =
                OsmHighway::parseSurfaceString(surfaceString);
            Speed surfaceSpeed =
                mConfig.getCostConfig().surfaceMaxSpeed.getSurfaceMaxSpeed(surface);
            if(surfaceSpeed < speed)
            {
                speed = surfaceSpeed;
            }
        }
        catch (OsmException& e)
        {
            throw MapProviderException(
                std::string("PostGisProvider:addTravelTime... ") +
                "could not parse surface " + surfaceString);
        }
    }
    double speed_mps = speed / 3.6;
    double travel_time = rEdge.geomData().length/ speed_mps;
    rEdge.edgeCost().addCost(EdgeCost::TRAVEL_TIME, travel_time);
    rEdge.setSpeed(speed);
}

void
PostGisProvider::addOtherCostToEdge(Edge& rEdge, const std::string& key)
{
    size_t eq_char = key.find('=');
    if((eq_char == std::string::npos) || (eq_char == key.length() - 1))
    {
        return;
    }

    Cost cost = mConfig.getCostConfig().otherEdgeCosts.getOtherCost(key);
    rEdge.edgeCost().addCost(EdgeCost::OTHER, cost);
}

Speed
PostGisProvider::getDefaultSpeedForEdge(const Edge& rEdge) const
{
    OsmHighway::HighwayType type = rEdge.roadData().roadType;
    const CostConfig& costConfig = mConfig.getCostConfig();
    Speed speed=
        costConfig.defaultSpeed.getDefaultSpeed(type, CostConfig::DefaultSpeed::LOW);
    return speed;
}

