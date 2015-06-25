/*
 * PostGisProvider.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "PostGisProvider.h"  // class implemented
#include "TopologyQueries.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

#include "../../graph/Edge.h"
#include "../../osm/OsmId.h"



/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
PostGisProvider::PostGisProvider(const Configuration& rConfig)
try
    : MapProvider(rConfig),
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
        mHighwayTableName   = nt.esc(mTopoConfig.roadsPrefix + "_" +
                                topoBaseName);
        mPointTableName     = nt.esc("planet_osm_point");
        mSchemaName         = nt.esc(mTopoConfig.topologySchemaPrefix + "_" +
                                topoBaseName);
        mEdgeTable          = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeTableName);
        mEdgeIdCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeIdColumnName);
        mSourceCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.sourceColumnName);
        mTargetCol          = nt.esc(mSchemaName + "." +
                                mTopoConfig.targetColumnName);
        mEdgeGeomCol        = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeGeomColumnName);
        mVertexTable        = nt.esc(mSchemaName + "." +
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
PostGisProvider::getRestrictions(
    Restrictions& rRestrictions,
    Topology& rTopology)
{
    getEdgeRestrictions(rRestrictions, rTopology);
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
		    mVertexTable);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopologyVertices: ") + e.what());
	}
}

void
PostGisProvider::addVertexResultToTopology(const pqxx::result& result,
                                           Topology& rTopology)
{
    for(size_t row = 0; row < result.size(); ++row)
    {
        VertexIdType
            id(result[row][TopologyQueries::VertexResult::NODE_ID].as<int>());
        Point
            point(result[row][TopologyQueries::VertexResult::X].as<double>(),
                  result[row][TopologyQueries::VertexResult::Y].as<double>());
        rTopology.addVertex(id, point);
    }
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
		    mEdgeTable,
		    mSchemaName,
		    mHighwayTableName);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getTopoEdges: ") + e.what());
	}
}

void
PostGisProvider::addEdgeResultToTopology(const pqxx::result& result,
                                         Topology& rTopology)
{
    for(const pqxx::tuple& row : result)
    {
        Edge& edge = addBasicResultToEdge(row, rTopology);
        addGeomDataResultToEdge(edge, row);
        addRoadDataResultToEdge(edge, row);
    }
}

Edge&
PostGisProvider::addBasicResultToEdge(const pqxx::tuple& rRow,
                                      Topology& rTopology)
{
    EdgeIdType
        edge_id(rRow[TopologyQueries::EdgeResult::EDGE_ID]
                .as<EdgeIdType>(Edge::MAX_ID));
    OsmIdType
        osm_id(rRow[TopologyQueries::EdgeResult::OSM_ID]
               .as<OsmIdType>(Osm::MAX_ID));
    VertexIdType
        source_id(rRow[TopologyQueries::EdgeResult::START_NODE]
                  .as<int>(Vertex::MAX_ID));
    VertexIdType
        target_id(rRow[TopologyQueries::EdgeResult::END_NODE]
                  .as<int>(Vertex::MAX_ID));
    Edge&
        edge = rTopology.addEdge(edge_id, osm_id, source_id, target_id);

    return edge;
}

void
PostGisProvider::addGeomDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::GeomData
        gd(rRow[TopologyQueries::EdgeResult::EDGE_LENGTH].as<double>(0),
           Point(rRow[TopologyQueries::EdgeResult::CENTER_X].as<double>(0),
                 rRow[TopologyQueries::EdgeResult::CENTER_Y].as<double>(0)),
           rRow[TopologyQueries::EdgeResult::SOURCE_BEARING].as<int>(0),
           rRow[TopologyQueries::EdgeResult::TARGET_BEARING].as<int>(0));
    rEdge.setGeomData(gd);
}

void
PostGisProvider::addRoadDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::RoadData rd;
    std::string
        onewayStr(rRow[TopologyQueries::EdgeResult::ONEWAY]
                  .as<std::string>("no"));

    if(rRow[TopologyQueries::EdgeResult::JUNCTION]
            .as<std::string>("") == OsmHighway::JUNCTION_ROUNDABOUT)
    {
        onewayStr = "yes";
    }
    if(onewayStr == "yes")
    {
        rd.direction = Edge::DirectionType::FROM_TO;
    }
    else if(onewayStr == "-1")
    {
        rd.direction = Edge::DirectionType::TO_FROM;
    }

    rd.nrLanes = rRow[TopologyQueries::EdgeResult::LANES].as<size_t>(1);

    addHighwayTypeToEdgeRoadData(rd, rRow);

    rEdge.setRoadData(rd);
}

void
PostGisProvider::addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                              const pqxx::tuple& rRow)
{
    std::string roadTypeStr(
        rRow[TopologyQueries::EdgeResult::HIGHWAY].as<std::string>("road"));
    try
    {
        rRoadData.roadType = OsmHighway::parseString(roadTypeStr);
    }
    catch (OsmException& oe)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addHighwayTypeToEdgeRoadData:")
            + oe.what());
    }
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
		    TopologyQueries::createTemporaryTable(transaction, mHighwayTableName);
		    TopologyQueries::createTemporarySchema(
		        transaction, mSchemaName, srid);
		    TopologyQueries::addTopoGeometryColumn(
		        transaction, mSchemaName, mHighwayTableName);
		    TopologyQueries::fillTopoGeometryColumn(
		        transaction, mSchemaName, mHighwayTableName, tolerance);

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
		    TopologyQueries::dropTemporaryTable(transaction, mHighwayTableName);
		    TopologyQueries::dropTemporarySchema(transaction, mSchemaName);
		    TopologyQueries::deleteTemporaryLayerRecord(transaction, mHighwayTableName);
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
PostGisProvider::getEdgeRestrictions(
    Restrictions& rRestrictions,
    Topology&     rTopology)
{
    pqxx::result result;
    getVehiclePropertyEdgeRestrictions(result);
    addVehiclePropertyResultToEdgeRestrictions(result, rRestrictions);

    result.clear();
    getAccessRestrictions(result);
    addAccessResultToEdgeRestrictions(result, rRestrictions);

    result.clear();
    getTurningRestrictions(result);
    addTurningResultToEdgeRestrictions(result, rRestrictions, rTopology);

    result.clear();
    getEdgePointRestrictions(result);
    addPointResultToEdgeRestrictions(result, rRestrictions, rTopology);
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
            mEdgeTable,
            mHighwayTableName,
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
PostGisProvider::addVehiclePropertyResultToEdgeRestrictions(
    const pqxx::result& rResult,
    Restrictions& rRestrictions)
{
    try
    {
        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();

        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[TopologyQueries::VehiclePropertiesResult::EDGE_ID]
                    .as<EdgeIdType>();

            EdgeRestrictions::VehicleProperties vp;
            vp.maxHeight =
                row[TopologyQueries::VehiclePropertiesResult::MAXHEIGHT].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxLength =
                row[TopologyQueries::VehiclePropertiesResult::MAXLENGTH].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxWeight =
                row[TopologyQueries::VehiclePropertiesResult::MAXWEIGHT].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxWidth =
                row[TopologyQueries::VehiclePropertiesResult::MAXWIDTH].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxSpeed =
                row[TopologyQueries::VehiclePropertiesResult::MAXSPEED].as<unsigned>
                (EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MAX);
            vp.minSpeed =
                row[TopologyQueries::VehiclePropertiesResult::MINSPEED].as<unsigned>
                (EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MIN);

            edgeRestr.setVehiclePropertyRestrictionForEdge(edgeId, vp);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addVehicleProp..ToEdge..: ") + e.what());

    }
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
            mEdgeTable,
            mHighwayTableName,
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
PostGisProvider::addAccessResultToEdgeRestrictions(
    const pqxx::result& rResult,
    Restrictions& rRestrictions)
{
    try
    {
        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();

        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[TopologyQueries::AccessResult::EDGE_ID].as<EdgeIdType>();

            std::string colString;
            colString = row[TopologyQueries::AccessResult::ACCESS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
            }

            colString = row[TopologyQueries::AccessResult::MOTORCAR]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTORCAR,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::GOODS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::GOODS,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::HGV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::HGV,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::PSV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::PSV,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::LHV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::LHV,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::MOTOR_VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTOR_VEHICLE,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::VEHICLE,
                    type
                );
            }

            colString = row[TopologyQueries::AccessResult::BARRIER]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmBarrier::BarrierType type = OsmBarrier::parseString(colString);
                edgeRestr.setBarrierRestrictionForEdge(edgeId, type);
            }

            colString = row[TopologyQueries::AccessResult::DISUSED]
                            .as<std::string>("");
            if(colString == "yes")
            {
                edgeRestr.setDisusedRestrictionForEdge(edgeId);
            }

            colString = row[TopologyQueries::AccessResult::NOEXIT]
                            .as<std::string>("");
            if(colString == "yes")
            {
                edgeRestr.setNoExitRestrictionForEdge(edgeId);
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
                mHighwayTableName,
                mEdgeTable);
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
PostGisProvider::addTurningResultToEdgeRestrictions(
    const pqxx::result&     rResult,
    Restrictions&           rRestrictions,
    Topology&               rTopology)
{
    try
    {
        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();

        for(const pqxx::tuple& row : rResult)
        {
            OsmTurningRestriction turn =
                RestrictionQueries::Results::
                    parseTurningRestrictionResultRow(row, rTopology);
            edgeRestr.addTurningRestrictionForEdge(turn.fromEdgeId(), turn);
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
		    mEdgeTable);
	}
	catch(const std::exception& e)
	{
        throw MapProviderException(
            std::string("PostGisProvider:getEdgePointRestrictions: ") + e.what());
	}
}

void
PostGisProvider::addPointResultToEdgeRestrictions(
    const pqxx::result&     rResult,
    Restrictions&           rRestrictions,
    Topology&               rTopology)
{
    try
    {
        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();

        for(const pqxx::tuple& row : rResult)
        {
            EdgeIdType edgeId =
                row[RestrictionQueries::EdgePointRestrictions::EDGE_ID]
                    .as<EdgeIdType>();

            std::string barrierTypeString =
                row[RestrictionQueries::EdgePointRestrictions::BARRIER]
                    .as<std::string>();
            OsmBarrier::BarrierType barrierType =
                OsmBarrier::parseString(barrierTypeString);
            edgeRestr.setBarrierRestrictionForEdge(edgeId, barrierType);

            // mark edge as having a restriction
            Edge& edge = rTopology.getEdge(edgeId);
            edge.setHasRestrictions(true);

            std::string colString;
            colString = row[RestrictionQueries::EdgePointRestrictions::ACCESS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::GOODS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::GOODS,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::HGV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::HGV,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::LHV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::LHV,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTORCAR]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTORCAR,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTOR_VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTOR_VEHICLE,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::PSV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::PSV,
                    type
                );
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::VEHICLE,
                    type
                );
            }
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addPointResultToEdge..: ") + e.what());
    }
}
