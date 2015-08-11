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

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

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
//      mEdgeResult()
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
//    getVerticesFromDb(mVertexResult);
//    getEdgesFromDb(mEdgeResult);

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



//void
//PostGisProvider::getRestrictions(
//    Restrictions& rRestrictions,
//    Topology& rTopology)
//{
//    getEdgeRestrictions(rRestrictions, rTopology);
//}


//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////

//void
//PostGisProvider::getVerticesFromDb(pqxx::result& rVertexResult)
//{
//	try
//	{
//		if(!mConnection.is_open())
//		{
//			throw MapProviderException(
//					std::string("Could not open ") + mDbConfig.database);
//		}
//
//		// NON-TRANSACTION START
//		pqxx::nontransaction transaction(mConnection);
//
//		rVertexResult = transaction.exec(
//				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
//				" FROM " + mVertexTable +
//				" ORDER BY node_id ASC;"
//		);
//	}
//	catch(const std::exception& e)
//	{
//        throw MapProviderException(
//            std::string("PostGisProvider:getTopologyVertices: ") + e.what());
//	}
//}
//
//void
//PostGisProvider::getEdgesFromDb(pqxx::result& rEdgeResult)
//{
//	try
//	{
//		if(!mConnection.is_open())
//		{
//			throw MapProviderException(
//					std::string("Could not open ") + mDbConfig.database);
//		}
//		std::string sql(
//		    "SELECT     edge_id, "
//		    "           start_node, "
//		    "           end_node, "
//		    //-- geom data about edge
//		    "           ST_Length(geom) AS edge_length, "
//		    "           ST_X(ST_LineInterpolatePoint(geom, 0.5)) AS center_x, "
//		    "           ST_Y(ST_LineInterpolatePoint(geom, 0.5)) AS center_y, "
//		    "           (ST_Azimuth("
//		    "               ST_PointN(geom,1), "
//		    "               ST_PointN(geom,2))/(2*pi())*360)::int "
//		    "               AS source_bearing, "
//		    "           (ST_Azimuth("
//		    "               ST_PointN(geom,ST_NPoints(geom)-1), "
//		    "               ST_PointN(geom,ST_NPoints(geom)))/(2*pi())*360)::int "
//		    "               AS target_bearing, "
//		    //-- osm data about original edge
//		    "           osm.* "
//		    "FROM      " + mEdgeTable +
//		    " JOIN ( "
//		    "   SELECT  rel.element_id, edges.* "
//		    //-- road data
////		    "           , highway "
////		    "           , junction "
////		    "           , lanes "
////		    "           , oneway "
//		    "   FROM    " + mSchemaName + ".relation rel"
//		    "   JOIN    " + mHighwayTableName + " AS edges"
//		    "   ON      topogeo_id = (topo_geom).id "
//		    "   WHERE   highway in " + OsmHighway::typesAsCommaSeparatedString() +
//		    ") AS osm "
//		    "ON edge_id = element_id "
//		    "ORDER BY edge_id ASC;"
//		);
//
//		// NON-TRANSACTION START
//		pqxx::nontransaction transaction(mConnection);
//
//		rEdgeResult = transaction.exec(sql);
//	}
//	catch(const std::exception& e)
//	{
//        throw MapProviderException(
//            std::string("PostGisProvider:getTopologyVertices: ") + e.what());
//	}
//
//}

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
//void
//PostGisProvider::addRestrictionsAndCosts(Topology& rTopology)
//{
//    addEdgeRestrictions(rTopology);
//}

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

//    addEdgeCosts(rTopology);

}

//void
//PostGisProvider::getEdgeRestrictions(
//    Restrictions& rRestrictions,
//    Topology&     rTopology)
//{
//    pqxx::result result;
//    getVehiclePropertyEdgeRestrictions(result);
//    addVehiclePropertyResultToEdgeRestrictions(result, rRestrictions, rTopology);
//
//    result.clear();
//    getAccessRestrictions(result);
//    addAccessResultToEdgeRestrictions(result, rRestrictions, rTopology);
//
//    result.clear();
//    getTurningRestrictions(result);
//    addTurningResultToEdgeRestrictions(result, rRestrictions, rTopology);
//
//    result.clear();
//    getEdgePointRestrictions(result);
//    addPointResultToEdgeRestrictions(result, rRestrictions, rTopology);
//}

//void
//PostGisProvider::addEdgeCosts(Topology& rTopology)
//{
//
//}

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
PostGisProvider::addVehiclePropertyRestrictionsToEdge(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[RestrictionQueries::VehiclePropertiesRestrictions::EDGE_ID]
                    .as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            EdgeRestriction::VehicleProperties* p_vp = new EdgeRestriction::VehicleProperties();

            p_vp->maxHeight =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MAXHEIGHT].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxLength =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MAXLENGTH].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxWeight =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MAXWEIGHT].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxWidth =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MAXWIDTH].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxSpeed =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MAXSPEED].as<unsigned>
                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
            p_vp->minSpeed =
                row[RestrictionQueries::VehiclePropertiesRestrictions::MINSPEED].as<unsigned>
                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MIN);

//            std::string surface_string =
//                row[TopologyQueries::VehiclePropertiesRestrictions::SURFACE].as<std::string>
//                ("");


            r_restrictions.setVehiclePropertyRestriction(p_vp);

//            addSpeedCost(edge, p_vp->maxSpeed, surface_string);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("PostGisProvider:addVehicleProp..ToEdge..: ") + e.what());
    }

}

//void
//PostGisProvider::addVehiclePropertyResultToEdgeRestrictions(
//    const pqxx::result& rResult,
//    Restrictions&       rRestrictions,
//    Topology&           rTopology)
//{
//    try
//    {
//        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();
//
//        for(const pqxx::tuple& row : rResult)
//        {
//            // throw exception if no edgeId
//            EdgeIdType edgeId =
//                row[TopologyQueries::VehiclePropertiesRestrictions::EDGE_ID]
//                    .as<EdgeIdType>();
//
//            EdgeRestrictions::VehicleProperties vp;
//            vp.maxHeight =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MAXHEIGHT].as<double>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            vp.maxLength =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MAXLENGTH].as<double>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            vp.maxWeight =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MAXWEIGHT].as<double>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            vp.maxWidth =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MAXWIDTH].as<double>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX);
//            vp.maxSpeed =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MAXSPEED].as<unsigned>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MAX);
//            vp.minSpeed =
//                row[TopologyQueries::VehiclePropertiesRestrictions::MINSPEED].as<unsigned>
//                (EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MIN);
//
//            edgeRestr.setVehiclePropertyRestrictionForEdge(edgeId, vp);
//
//            // mark edge as having a restriction
//            Edge& edge = rTopology.getEdge(edgeId);
//            edge.setHasRestrictions(true);
//        }
//    }
//    catch (std::exception& e)
//    {
//        throw MapProviderException(
//            std::string("PostGisProvider:addVehicleProp..ToEdge..: ") + e.what());
//    }
//}

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
                row[RestrictionQueries::AccessRestrictions::EDGE_ID].as<EdgeIdType>();

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

//void
//PostGisProvider::addAccessResultToEdgeRestrictions(
//    const pqxx::result& rResult,
//    Restrictions&       rRestrictions,
//    Topology&           rTopology)
//{
//    try
//    {
//        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();
//
//        for(const pqxx::tuple& row : rResult)
//        {
//            // throw exception if no edgeId
//            EdgeIdType edgeId =
//                row[TopologyQueries::AccessRestrictions::EDGE_ID].as<EdgeIdType>();
//
//            std::string colString;
//            colString = row[TopologyQueries::AccessRestrictions::ACCESS]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::MOTORCAR]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTORCAR,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::GOODS]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::GOODS,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::HGV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::HGV,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::PSV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::PSV,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::LHV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::LHV,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::MOTOR_VEHICLE]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTOR_VEHICLE,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::VEHICLE]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::VEHICLE,
//                    type
//                );
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::BARRIER]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmBarrier::BarrierType type = OsmBarrier::parseString(colString);
//                edgeRestr.setBarrierRestrictionForEdge(edgeId, type);
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::DISUSED]
//                            .as<std::string>("");
//            if(colString == "yes")
//            {
//                edgeRestr.setDisusedRestrictionForEdge(edgeId);
//            }
//
//            colString = row[TopologyQueries::AccessRestrictions::NOEXIT]
//                            .as<std::string>("");
//            if(colString == "yes")
//            {
//                edgeRestr.setNoExitRestrictionForEdge(edgeId);
//            }
//
//            // mark edge as having a restriction
//            Edge& edge = rTopology.getEdge(edgeId);
//            edge.setHasRestrictions(true);
//        }
//    }
//    catch (std::exception& e)
//    {
//        throw MapProviderException(
//            std::string("PostGisProvider:addAccessResultToEdge..: ") + e.what());
//    }
//}

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
PostGisProvider::addTurningRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            OsmTurningRestriction* p_turn =
                RestrictionQueries::Results::
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

//void
//PostGisProvider::addTurningResultToEdgeRestrictions(
//    const pqxx::result&     rResult,
//    Restrictions&           rRestrictions,
//    Topology&               rTopology)
//{
//    try
//    {
//        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();
//
//        for(const pqxx::tuple& row : rResult)
//        {
//            OsmTurningRestriction turn =
//                RestrictionQueries::Results::
//                    parseTurningRestrictionResultRow(row, rTopology);
//            edgeRestr.addTurningRestrictionForEdge(turn.fromEdgeId(), turn);
//
//            // mark edge as having a restriction
//            Edge& edge = rTopology.getEdge(turn.fromEdgeId());
//            edge.setHasRestrictions(true);
//        }
//    }
//    catch (std::exception& e)
//    {
//        throw MapProviderException(
//            std::string("PostGisProvider:addTurningResultToEdge..: ") + e.what());
//    }
//}

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
//            edgeRestr.setBarrierRestrictionForEdge(edgeId, barrierType);

//            // mark edge as having a restriction
//            Edge& edge = rTopology.getEdge(edgeId);
//            edge.setHasRestrictions(true);

            std::string colString;
            colString = row[RestrictionQueries::EdgePointRestrictions::ACCESS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.setGeneralAccessRestriction(type);
//                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::GOODS]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::GOODS,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::GOODS,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::HGV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::HGV,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::HGV,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::LHV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::LHV,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::LHV,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTORCAR]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTORCAR,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTORCAR,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::MOTOR_VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTOR_VEHICLE,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTOR_VEHICLE,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::PSV]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::PSV,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::PSV,
//                    type);
            }

            colString = row[RestrictionQueries::EdgePointRestrictions::VEHICLE]
                            .as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::VEHICLE,
                    type);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::VEHICLE,
//                    type);
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

    result.clear();
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
PostGisProvider::addTravelTimeCosts(const pqxx::result& rResult, Topology& rTopology)
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

//            std::string highway_string =
//                row[CostQueries::TravelTimeCostResult::HIGHWAY].as<std::string>("");
//            OsmHighway::HighwayType highwayType =
//                edge.roadData().roadType;
//                OsmHighway::parseString(highway_string);
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
PostGisProvider::addTravelTimeCostToEdge(Edge& rEdge, Speed speed, std::string& surfaceString)
{
    bool hasMaxSpeed = (speed != EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
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
            throw MapProviderException(std::string("PostGisProvider:addTravelTime... ") +
                "could not parse surface " + surfaceString);
        }
    }
    double speed_mps = speed / 3.6;
    double travel_time = rEdge.geomData().length/ speed_mps;
//    if(rEdge.id() == 270)
//    {
//        std::cerr << "Edge " << rEdge.id()
//            << ": speed (km/h): " << speed
//            << ", (m/s): " << speed_mps
//            << ", traveltime (s):" << travel_time << std::endl;
//    }
    rEdge.edgeCost().addCost(EdgeCost::TRAVEL_TIME, travel_time);
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

//void
//PostGisProvider::addPointResultToEdgeRestrictions(
//    const pqxx::result&     rResult,
//    Restrictions&           rRestrictions,
//    Topology&               rTopology)
//{
//    try
//    {
//        EdgeRestrictions& edgeRestr = rRestrictions.edgeRestrictions();
//
//        for(const pqxx::tuple& row : rResult)
//        {
//            EdgeIdType edgeId =
//                row[RestrictionQueries::EdgePointRestrictions::EDGE_ID]
//                    .as<EdgeIdType>();
//
//            std::string barrierTypeString =
//                row[RestrictionQueries::EdgePointRestrictions::BARRIER]
//                    .as<std::string>();
//            OsmBarrier::BarrierType barrierType =
//                OsmBarrier::parseString(barrierTypeString);
//            edgeRestr.setBarrierRestrictionForEdge(edgeId, barrierType);
//
//            // mark edge as having a restriction
//            Edge& edge = rTopology.getEdge(edgeId);
//            edge.setHasRestrictions(true);
//
//            std::string colString;
//            colString = row[RestrictionQueries::EdgePointRestrictions::ACCESS]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.setGeneralAccessRestrictionForEdge(edgeId, type);
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::GOODS]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::GOODS,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::HGV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::HGV,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::LHV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::LHV,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::MOTORCAR]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTORCAR,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::MOTOR_VEHICLE]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::MOTOR_VEHICLE,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::PSV]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::PSV,
//                    type
//                );
//            }
//
//            colString = row[RestrictionQueries::EdgePointRestrictions::VEHICLE]
//                            .as<std::string>("");
//            if(colString != "")
//            {
//                OsmAccess::AccessType type = OsmAccess::parseString(colString);
//                edgeRestr.addVehicleTypeAccessRestrictionsForEdge(
//                    edgeId,
//                    OsmVehicle::VEHICLE,
//                    type
//                );
//            }
//        }
//    }
//    catch (std::exception& e)
//    {
//        throw MapProviderException(
//            std::string("PostGisProvider:addPointResultToEdge..: ") + e.what());
//    }
//}
