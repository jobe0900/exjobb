/*
 * RestrictionQueries.cc
 *
 *  Created on: 2015-05-31
 *      Author: Jonas Bergman
 */


#include "RestrictionQueries.h"  // class implemented

#include <iostream>

// Result --------------------------------------------------------------------
//static
OsmTurningRestriction*
RestrictionQueries::TurningRestrictions::
    Results::parseTurningRestrictionResultRow(
    const pqxx::tuple&    rRow,
    Topology&             rTopology)
{
    OsmIdType fromOsmId =
        rRow[RestrictionQueries::TurningRestrictions::FROM_OSM_ID].as<OsmIdType>();
    OsmIdType toOsmId =
        rRow[RestrictionQueries::TurningRestrictions::TO_OSM_ID].as<OsmIdType>();
    std::string typeString =
        rRow[RestrictionQueries::TurningRestrictions::RESTRICTION_TYPE].as<std::string>();
    OsmTurningRestriction::TurningRestrictionType type =
        OsmTurningRestriction::parseString(typeString);
    std::string edgeIdsString =
        rRow[RestrictionQueries::TurningRestrictions::EDGE_IDS].as<std::string>();
    std::string viaOsmIdsString =
        rRow[RestrictionQueries::TurningRestrictions::VIA_OSM].as<std::string>("");

    std::vector<EdgeIdType> edgeIds = parseEdgeIdsString(edgeIdsString);

    Edge& fromEdge = findEdgeMatchingOsmId(fromOsmId, edgeIds, rTopology);
    Edge& toEdge   = findEdgeMatchingOsmId(toOsmId, edgeIds, rTopology);

    OsmTurningRestriction* p_restriction {nullptr};

    // VIA WAY
    if(fromEdge.targetId() != toEdge.sourceId())
    {
        p_restriction = new OsmTurningRestriction(
            type,
            fromEdge.id(),
            viaOsmIdsString,
            toEdge.id());
    }
    // VIA NODE
    else
    {
        VertexIdType vertexId = fromEdge.targetId();
        p_restriction = new OsmTurningRestriction(
            type,
            fromEdge.id(),
            vertexId,
            toEdge.id());
    }
    return p_restriction;
}

//static
std::vector<EdgeIdType>
RestrictionQueries::TurningRestrictions::Results::parseEdgeIdsString(
    std::string& rEdgeIds)
{
    boost::trim_if(rEdgeIds, boost::is_any_of("{}"));
    std::vector<std::string> idStrings;
    boost::split(idStrings, rEdgeIds, boost::is_any_of(","));

    std::vector<EdgeIdType> edgeIds;
    for(const std::string& idStr : idStrings)
    {
        edgeIds.push_back(Edge::parse(idStr));
    }
    return edgeIds;
}

//static
Edge&
RestrictionQueries::TurningRestrictions::Results::findEdgeMatchingOsmId(
    OsmIdType                       osmId,
    const std::vector<EdgeIdType>&  rEdgeIds,
    Topology&                       rTopology)
{
    for(EdgeIdType id : rEdgeIds)
    {
        Edge& edge = rTopology.getEdge(id);
        if(edge.osmId() == osmId)
        {
            return edge;
        }
    }
    throw MapProviderException(
        "PostGisRestrictionQueries:Result:findEdgeMatchingOsmId: "
        "No edges matching osm_id: " + std::to_string(osmId));
}

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
void
RestrictionQueries::getVehiclePropertyEdgeRestrictions(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    std::vector<std::string> columns {
        "maxheight",
        "maxlength",
        "maxweight",
        "maxwidth",
        "maxspeed",
        "minspeed"
    };

    rResult = rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
        notNullColumns(columns) +
        endOfQuery()
    );
}

// static
void
RestrictionQueries::addVehiclePropertyRestrictionsToEdge(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[VehiclePropertiesRestrictions::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            EdgeRestriction::VehicleProperties* p_vp =
                new EdgeRestriction::VehicleProperties();

            p_vp->maxHeight =
                row[VehiclePropertiesRestrictions::MAXHEIGHT].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxLength =
                row[VehiclePropertiesRestrictions::MAXLENGTH].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxWeight =
                row[VehiclePropertiesRestrictions::MAXWEIGHT].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxWidth =
                row[VehiclePropertiesRestrictions::MAXWIDTH].as<double>
                (EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX);
            p_vp->maxSpeed =
                row[VehiclePropertiesRestrictions::MAXSPEED].as<unsigned>
                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX);
            p_vp->minSpeed =
                row[VehiclePropertiesRestrictions::MINSPEED].as<unsigned>
                (EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MIN);

            r_restrictions.setVehiclePropertyRestriction(p_vp);
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("RestrictionQueries:addVehicleProp..ToEdge..: ")
                        + e.what());
    }
}

//static
void
RestrictionQueries::getAccessRestrictions(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    std::vector<std::string> columns {
        "access",
        "barrier",
        "disused",
        "noexit",
        "motorcar",
        "goods",
        "hgv",
        "psv",
        "lhv",
        "motor_vehicle",
        "vehicle"
    };

    rResult = rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
        notNullColumns(columns) +
        endOfQuery()
    );
}

// static
void
RestrictionQueries::addAccessRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology,
    const Configuration&    rConfig)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[AccessRestrictions::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            std::string colString;
            colString = row[AccessRestrictions::ACCESS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.setGeneralAccessRestriction(type);
            }

            colString = row[AccessRestrictions::MOTORCAR].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTORCAR,
                    type
                );
            }

            colString = row[AccessRestrictions::GOODS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::GOODS,
                    type
                );
            }

            colString = row[AccessRestrictions::HGV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::HGV,
                    type
                );
            }

            colString = row[AccessRestrictions::PSV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::PSV,
                    type
                );
            }

            colString = row[AccessRestrictions::LHV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::LHV,
                    type
                );
            }

            colString = row[AccessRestrictions::MOTOR_VEHICLE].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTOR_VEHICLE,
                    type
                );
            }

            colString = row[AccessRestrictions::VEHICLE].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::VEHICLE,
                    type
                );
            }

            colString = row[AccessRestrictions::BARRIER].as<std::string>("");
            if(colString != "")
            {
                OsmBarrier::BarrierType type = OsmBarrier::parseString(colString);
                r_restrictions.setBarrierRestriction(type);
                CostQueries::addBarrierCostToEdge(edge, type, rConfig);
            }

            colString = row[AccessRestrictions::DISUSED].as<std::string>("");
            if(colString == "yes")
            {
                r_restrictions.setDisusedRestriction();
            }

            colString = row[AccessRestrictions::NOEXIT].as<std::string>("");
            if(colString == "yes")
            {
                r_restrictions.setNoExitRestriction();
            }
        }
    }
    catch (std::exception& e)
    {
        throw MapProviderException(
            std::string("RestrictionQueries:addAccessResultToEdge..: ") + e.what());
    }
}

//static
void
RestrictionQueries::dropCreateTurningRestrictionsTable(
    pqxx::transaction_base& rTrans)
{
    rTrans.exec(
        "DROP TABLE IF EXISTS turning_restrictions; "
        "CREATE TABLE turning_restrictions( "
        "   from_osm_id       bigint, "
        "   to_osm_id         bigint, "
        "   via_osm           varchar, "
        "   edge_ids          integer[], "
        "   restriction_type  varchar)"
    );
}

//static
void
RestrictionQueries::identifyTurningRestrictions(
    pqxx::transaction_base& rTrans,
    const std::string&      rOsmEdgeTable,
    const std::string&      rTopoEdgeTable)
{
    rTrans.exec(
        "SELECT * FROM find_osm_turning_restrictions('"
        + rOsmEdgeTable + "', '" + rTopoEdgeTable + "')"
    );
}

//static
void
RestrictionQueries::getTurningRestrictions(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult)
{
    rResult = rTrans.exec(
        "SELECT * FROM turning_restrictions"
    );
}

// static
void
RestrictionQueries::addTurningRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology)
{
    try
    {
        for(const pqxx::tuple& row : rResult)
        {
            OsmTurningRestriction* p_turn =
                TurningRestrictions::Results::
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
            std::string("RestrictionQueries:addTurningResultToEdge..: ") + e.what());
    }
}

//static
void
RestrictionQueries::getEdgePointRestrictions(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rOsmPointTable,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    rResult = rTrans.exec(
        "SELECT p.osm_id, "
        "       p.barrier, "
        "       p.access, "
        "       p.goods, "
        "       p.hgv, "
        "       p.lhv, "
        "       p.motorcar, "
        "       p.motor_vehicle, "
        "       p.psv, "
        "       p.vehicle, "
        "       t.edge_id "
        "FROM  " + rOsmPointTable + " p, "
        "      " + rTopoEdgeTable + " t, "
        "      " + rOsmEdgeTable + " o, "
        "      " + rSchemaName + ".relation r "
        "WHERE  r.topogeo_id = (topo_geom).id "
        "AND    r.element_id = t.edge_id "
        "AND    p.barrier IS NOT NULL "
        "AND    ST_Intersects(p.way, t.geom) "
        "AND    o.highway IN " + OsmHighway::typesAsCommaSeparatedString()
    );
}
// static
void
RestrictionQueries::addPointRestrictionsToEdge(
    const pqxx::result&     rResult,
    Topology&               rTopology,
    const Configuration&   rConfig)
{
    try
    {
//        std::cerr << "Number of point restrictions: "<< rResult.size() << std::endl;

        for(const pqxx::tuple& row : rResult)
        {
            // throw exception if no edgeId
            EdgeIdType edgeId =
                row[EdgePointRestrictions::EDGE_ID].as<EdgeIdType>();

            Edge& edge = rTopology.getEdge(edgeId);
            EdgeRestriction& r_restrictions = edge.restrictions();

            std::string barrierTypeString =
                row[EdgePointRestrictions::BARRIER].as<std::string>();
            OsmBarrier::BarrierType barrierType =
                OsmBarrier::parseString(barrierTypeString);
            r_restrictions.setBarrierRestriction(barrierType);
            CostQueries::addBarrierCostToEdge(edge, barrierType, rConfig);

//            std::cerr << "Edge: " << edgeId
//                << " has barrier: " << barrierTypeString << std::endl;

            std::string colString;
            colString = row[EdgePointRestrictions::ACCESS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.setGeneralAccessRestriction(type);
            }

            colString = row[EdgePointRestrictions::GOODS].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::GOODS,
                    type);
            }

            colString = row[EdgePointRestrictions::HGV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::HGV,
                    type);
            }

            colString = row[EdgePointRestrictions::LHV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::LHV,
                    type);
            }

            colString = row[EdgePointRestrictions::MOTORCAR].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTORCAR,
                    type);
            }

            colString = row[EdgePointRestrictions::MOTOR_VEHICLE].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::MOTOR_VEHICLE,
                    type);
            }

            colString = row[EdgePointRestrictions::PSV].as<std::string>("");
            if(colString != "")
            {
                OsmAccess::AccessType type = OsmAccess::parseString(colString);
                r_restrictions.addVehicleTypeAccessRestriction(
                    OsmVehicle::PSV,
                    type);
            }

            colString = row[EdgePointRestrictions::VEHICLE].as<std::string>("");
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
            std::string("RestrictionQueries:addPointResultToEdge..: ") + e.what());
    }
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////
//static
std::string
RestrictionQueries::startOfQuery(const std::string& rTopoEdgeTable)
{
    return (
        "SELECT     edge_id, "
        //-- osm data about original edge
        "           osm.* "
        "FROM      " + rTopoEdgeTable +
        " JOIN ( "
        "   SELECT  element_id "
    );
}

//static
std::string
RestrictionQueries::queryColumns(const std::vector<std::string>& rCols)
{
    std::ostringstream oss;
    for(const std::string& col : rCols)
    {
        oss << ", " << col;
    }
    return oss.str();
}

//static
std::string
RestrictionQueries::midOfQuery(
    const std::string& rSchemaName,
    const std::string& rOsmEdgeTable)
{
    return (
        "   FROM    " + rSchemaName + ".relation "
        "   JOIN    " + rOsmEdgeTable +
        "   ON      topogeo_id = (topo_geom).id "
        "   WHERE   highway in " + OsmHighway::typesAsCommaSeparatedString()
    );
}

//static
std::string
RestrictionQueries::notNullColumns(const std::vector<std::string>& rCols)
{
    std::ostringstream oss;
    oss << " AND (";
    size_t i = 0;
    for(const std::string& col : rCols)
    {
        oss << col << " IS NOT NULL ";
        if(i < (rCols.size() - 1))
        {
            oss << " OR ";
        }
        ++i;
    }
    oss << ") ";
    return oss.str();
}

//static
std::string
RestrictionQueries::endOfQuery()
{
    return (
        ") AS osm "
        "ON edge_id = element_id "
        "ORDER BY edge_id ASC;"
    );
}


