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
        mTableName     = nt.esc(mTopoConfig.roadsPrefix + "_" +
                                topoBaseName);
        mSchemaName    = nt.esc(mTopoConfig.topologySchemaPrefix + "_" +
                                topoBaseName);
        mEdgeTable     = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeTableName);
        mEdgeIdCol     = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeIdColumnName);
        mSourceCol     = nt.esc(mSchemaName + "." +
                                mTopoConfig.sourceColumnName);
        mTargetCol     = nt.esc(mSchemaName + "." +
                                mTopoConfig.targetColumnName);
        mEdgeGeomCol   = nt.esc(mSchemaName + "." +
                                mTopoConfig.edgeGeomColumnName);
        mVertexTable   = nt.esc(mSchemaName + "." +
                                mTopoConfig.vertexTableName);
        mVertexIdCol   = nt.esc(mSchemaName + "." +
                                mTopoConfig.vertexIdColumnName);
        mVertexGeomCol = nt.esc(mSchemaName + "." +
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
        VertexIdType	id(result[row][0].as<int>());
        Point 		    point(result[row][1].as<double>(),
                              result[row][2].as<double>());
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
		    mTableName);

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

//bool
//PostGisProvider::validDimensionRestrictions(const pqxx::tuple& rRow) const
//{
//    const VehicleConfig& config = mrConfig.getVehicleConfig();
//    const double doubleMax(std::numeric_limits<double>::max());
//
//    if(config.height >= rRow[EdgeQueryResult::MAXHEIGHT].as<double>(doubleMax))
//        return false;
//    if(config.length >= rRow[EdgeQueryResult::MAXLENGTH].as<double>(doubleMax))
//        return false;
//    if(config.weight >= rRow[EdgeQueryResult::MAXWEIGHT].as<double>(doubleMax))
//        return false;
//    if(config.width  >= rRow[EdgeQueryResult::MAXWIDTH].as<double>(doubleMax))
//        return false;
//
//    return true;
//}

//bool
//PostGisProvider::accessAllowed(const pqxx::tuple& rRow) const
//{
//    OsmAccess generic_access(genericAccess(rRow));
//    OsmAccess type_access(vehicleTypeAccessType(rRow));

    // generic allowed, but type restricted
//    if(generic_access == OsmAccess::AccessType::ALLOWED)
//    {
//        if(type_access == OsmAccess::AccessType::RESTRICTED)
//        {
//            return false;
//        }
//        return true;
//    }
//    // generic restricted, but type allowed
//    else if(generic_access == OsmAccess::AccessType::RESTRICTED)
//    {
//        if(type_access == OsmAccess::AccessType::ALLOWED)
//        {
//            return true;
//        }
//        return false;
//    }
//    // generic unspecified, but type restricted
//    else if(type_access == OsmAccess::AccessType::RESTRICTED)
//    {
//        return false;
//    }
    // generic and type unspecified == allowed
//    return true;
//}

//OsmConstants::AccessType
//PostGisProvider::genericAccess(const pqxx::tuple& rRow) const
//{
//    return accessTypeFromRestrictionString(
//        rRow[EdgeQueryResult::ACCESS].as<std::string>(""));
//}

//OsmConstants::AccessType
//PostGisProvider::vehicleTypeAccessType(const pqxx::tuple& rRow) const
//{
//    const OsmVehicle::VehicleType& v_type = mrConfig.getVehicleConfig().category;
//
//    std::string restriction_string("");
//    switch(v_type)
//    {
//        case OsmVehicle::MOTORCAR:
//            restriction_string =
//                rRow[EdgeQueryResult::MOTORCAR].as<std::string>(""); break;
//        case OsmVehicle::GOODS:
//            restriction_string =
//                rRow[EdgeQueryResult::GOODS].as<std::string>(""); break;
//        case OsmVehicle::HGV:
//            restriction_string =
//                rRow[EdgeQueryResult::HGV].as<std::string>(""); break;
//        case OsmVehicle::PSV:
//            restriction_string =
//                rRow[EdgeQueryResult::PSV].as<std::string>(""); break;
//        case OsmVehicle::LHV:
//            restriction_string =
//                rRow[EdgeQueryResult::LHV].as<std::string>(""); break;
//        case OsmVehicle::MOTOR_VEHICLE:
//            restriction_string =
//                rRow[EdgeQueryResult::MOTOR_VEHICLE].as<std::string>(""); break;
//        case OsmVehicle::VEHICLE:
//        default:
//            restriction_string =
//                rRow[EdgeQueryResult::VEHICLE].as<std::string>(""); break;
//
//    }
//    return accessTypeFromRestrictionString(restriction_string);
//}

//OsmAccess::AccessType
//PostGisProvider::accessTypeFromRestrictionString(
//        const std::string& rRestrictionString) const
//{
//    if(rRestrictionString != "")
//    {
//        if( rRestrictionString == OsmAccess::ACCESS_YES ||
//            rRestrictionString == ACCESS_PERMISSIVE ||
//            rRestrictionString == ACCESS_DESIGNATED)
//        {
//            return ALLOWED;
//        }
//        else
//        {
//            return RESTRICTED;
//        }
//    }
//    return NOT_SPECIFIED;
//}

Edge&
PostGisProvider::addBasicResultToEdge(const pqxx::tuple& rRow,
                                      Topology& rTopology)
{
    EdgeIdType    edge_id(rRow[EdgeQueryResult::EDGE_ID].as<EdgeIdType>(
                        std::numeric_limits<EdgeIdType>::max()));
    OsmIdType     osm_id(rRow[EdgeQueryResult::OSM_ID].as<OsmIdType>(
                        std::numeric_limits<OsmIdType>::max()));
    VertexIdType  source_id(rRow[EdgeQueryResult::START_NODE].as<int>(
                        std::numeric_limits<VertexIdType>::max()));
    VertexIdType  target_id(rRow[EdgeQueryResult::END_NODE].as<int>(
                        std::numeric_limits<VertexIdType>::max()));
    Edge&         edge = rTopology.addEdge(
                        edge_id, osm_id, source_id, target_id);

    return edge;
}

void
PostGisProvider::addGeomDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::GeomData  gd(rRow[EdgeQueryResult::EDGE_LENGTH].as<double>(0),
                       Point(rRow[EdgeQueryResult::CENTER_X].as<double>(0),
                             rRow[EdgeQueryResult::CENTER_Y].as<double>(0)),
                       rRow[EdgeQueryResult::SOURCE_BEARING].as<int>(0),
                       rRow[EdgeQueryResult::TARGET_BEARING].as<int>(0));
    rEdge.setGeomData(gd);
}

void
PostGisProvider::addRoadDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::RoadData rd;
    std::string onewayStr(rRow[EdgeQueryResult::ONEWAY].as<std::string>("no"));

    if(rRow[EdgeQueryResult::JUNCTION].as<std::string>("") == OsmHighway::JUNCTION_ROUNDABOUT)
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

    rd.nrLanes = rRow[EdgeQueryResult::LANES].as<size_t>(1);

    addHighwayTypeToEdgeRoadData(rd, rRow);

    rEdge.setRoadData(rd);
}

void
PostGisProvider::addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                              const pqxx::tuple& rRow)
{
    std::string roadTypeStr(rRow[EdgeQueryResult::HIGHWAY].as<std::string>("road"));
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
		    installPostgisTopology(transaction);
		    setSearchPath(transaction);
		    createTemporaryTable(transaction, mTableName);
		    createTemporarySchema(transaction, mSchemaName, srid);
		    addTopoGeometryColumn(transaction, mSchemaName, mTableName);
		    fillTopoGeometryColumn(transaction, mSchemaName, mTableName, tolerance);

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
		    dropTemporaryTable(transaction, mTableName);
		    dropTemporarySchema(transaction, mSchemaName);
		    deleteTemporaryLayerRecord(transaction, mTableName);
		    deleteTemporaryTopoRecord(transaction, mSchemaName);

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

void
PostGisProvider::installPostgisTopology(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"CREATE EXTENSION IF NOT EXISTS postgis_topology"
	);
}

void
PostGisProvider::setSearchPath(pqxx::transaction_base& rTrans)
{
	rTrans.exec(
			"SET search_path = topology, public"
	);
}

void
PostGisProvider::createTemporaryTable(pqxx::transaction_base& rTrans,
									  const std::string& rTableName)
{
	rTrans.exec(
			"CREATE TABLE public." + rTableName + " " +
			"AS SELECT * "
			"FROM planet_osm_line "
			"WHERE highway IS NOT NULL"
	);
}

void
PostGisProvider::createTemporarySchema(pqxx::transaction_base& rTrans,
									  const std::string& rSchemaName, int srid)
{
	rTrans.exec(
			"SELECT topology.CreateTopology('" +
			rSchemaName + "'," +
			rTrans.quote(srid) + ")"
	);
}

void
PostGisProvider::addTopoGeometryColumn(pqxx::transaction_base& rTrans,
									   const std::string& rSchemaName,
									   const std::string& rTableName)
{
	rTrans.exec(
			"SELECT topology.AddTopoGeometryColumn('" +
			rSchemaName + "', " +
			"'public', '" +
			rTableName + "', " +
			"'topo_geom', 'LINESTRING')"
	);
}

void
PostGisProvider::fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
									    const std::string& rSchemaName,
									    const std::string& rTableName,
										double tolerance)
{
	rTrans.exec(
			"UPDATE public." +
			rTableName + " " +
			"SET topo_geom = topology.toTopoGeom(way, '" +
			rSchemaName +
			"', 1, " +
			rTrans.quote(tolerance) + ")"
	);
}

void
PostGisProvider::dropTemporaryTable(pqxx::transaction_base& rTrans,
									const std::string& rTableName)
{
	rTrans.exec(
			"DROP TABLE IF EXISTS public." + rTableName
	);
}

void
PostGisProvider::dropTemporarySchema(pqxx::transaction_base& rTrans,
									 const std::string& rSchemaName)
{
	rTrans.exec(
			"DROP SCHEMA IF EXISTS " + rSchemaName + " CASCADE"
	);
}

void
PostGisProvider::deleteTemporaryLayerRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rTableName)
{
	rTrans.exec(
			"DELETE FROM topology.layer "
			"WHERE table_name = " + rTrans.quote(rTableName)
	);
}

void
PostGisProvider::deleteTemporaryTopoRecord(pqxx::transaction_base& rTrans,
									 	 	 const std::string& rSchemaName)
{
	rTrans.exec(
			"DELETE FROM topology.topology "
			"WHERE name = " + rTrans.quote(rSchemaName)
	);
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
            mTableName,
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
                row[VehiclePropertiesQueryResult::EDGE_ID].as<EdgeIdType>();

            EdgeRestrictions::VehicleProperties vp;
            vp.maxHeight =
                row[VehiclePropertiesQueryResult::MAXHEIGHT].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxLength =
                row[VehiclePropertiesQueryResult::MAXLENGTH].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxWeight =
                row[VehiclePropertiesQueryResult::MAXWEIGHT].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxWidth =
                row[VehiclePropertiesQueryResult::MAXWIDTH].as<double>
                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
            vp.maxSpeed =
                row[VehiclePropertiesQueryResult::MAXSPEED].as<unsigned>
                (EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MAX);
            vp.minSpeed =
                row[VehiclePropertiesQueryResult::MINSPEED].as<unsigned>
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
            mTableName,
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
                row[AccessQueryResult::EDGE_ID].as<EdgeIdType>();

            std::string colString;
            colString = row[AccessQueryResult::ACCESS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
            }

            colString = row[AccessQueryResult::MOTORCAR].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTORCAR,
                    type
                );
            }

            colString = row[AccessQueryResult::GOODS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::GOODS,
                    type
                );
            }

            colString = row[AccessQueryResult::HGV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::HGV,
                    type
                );
            }

            colString = row[AccessQueryResult::PSV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::PSV,
                    type
                );
            }

            colString = row[AccessQueryResult::LHV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::LHV,
                    type
                );
            }

            colString = row[AccessQueryResult::MOTOR_VEHICLE].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::MOTOR_VEHICLE,
                    type
                );
            }

            colString = row[AccessQueryResult::VEHICLE].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
                    edgeId,
                    OsmVehicle::VEHICLE,
                    type
                );
            }

            colString = row[AccessQueryResult::BARRIER].as<std::string>("");
            if(colString != "")
            {
                OsmBarrier::BarrierType type = OsmBarrier::parseString(colString);
                edgeRestr.setBarrierRestrictionForEdge(edgeId, type);
            }

            colString = row[AccessQueryResult::DISUSED].as<std::string>("");
            if(colString == "yes")
            {
                edgeRestr.setDisusedRestrictionForEdge(edgeId);
            }

            colString = row[AccessQueryResult::NOEXIT].as<std::string>("");
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
                mTableName,
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
