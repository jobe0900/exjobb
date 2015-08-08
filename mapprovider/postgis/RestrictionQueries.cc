/*
 * RestrictionQueries.cc
 *
 *  Created on: 2015-05-31
 *      Author: Jonas Bergman
 */


#include "RestrictionQueries.h"  // class implemented

// Result --------------------------------------------------------------------
//static
OsmTurningRestriction*
RestrictionQueries::Results::parseTurningRestrictionResultRow(
    const pqxx::tuple&    rRow,
    Topology&             rTopology)
{
    OsmIdType fromOsmId =
        rRow[RestrictionQueries::FROM_OSM_ID].as<OsmIdType>();
    OsmIdType toOsmId =
        rRow[RestrictionQueries::TO_OSM_ID].as<OsmIdType>();
    std::string typeString =
        rRow[RestrictionQueries::RESTRICTION_TYPE].as<std::string>();
    OsmTurningRestriction::TurningRestrictionType type =
        OsmTurningRestriction::parseString(typeString);
    std::string edgeIdsString =
        rRow[RestrictionQueries::EDGE_IDS].as<std::string>();
    std::string viaOsmIdsString =
        rRow[RestrictionQueries::VIA_OSM].as<std::string>("");

    std::vector<EdgeIdType> edgeIds = parseEdgeIdsString(edgeIdsString);

    Edge& fromEdge = findEdgeMatchingOsmId(fromOsmId, edgeIds, rTopology);
    Edge& toEdge   = findEdgeMatchingOsmId(toOsmId, edgeIds, rTopology);

    OsmTurningRestriction* p_restriction {nullptr};

    // VIA WAY
    if(fromEdge.target() != toEdge.source())
    {
//        fromEdge.setHasRestrictions(true);
//        fromEdge.setHasViaWayRestriction(true);
        p_restriction = new OsmTurningRestriction(
            type,
            fromEdge.id(),
            viaOsmIdsString,
            toEdge.id());
    }
    // VIA NODE
    else
    {
        VertexIdType vertexId = fromEdge.target();
//        fromEdge.setHasRestrictions(true);
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
RestrictionQueries::Results::parseEdgeIdsString(std::string& rEdgeIds)
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
RestrictionQueries::Results::findEdgeMatchingOsmId(
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
        "minspeed",
        "surface"
    };

    rResult = rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
        notNullColumns(columns) +
        endOfQuery()
    );
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

//static
void
RestrictionQueries::getEdgePointRestrictions(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rOsmPointTable,
    const std::string&      rTopoEdgeTable)
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
        "       e.edge_id "
        "FROM  " + rOsmPointTable + " p, "
        "      " + rTopoEdgeTable + " e "
        "WHERE  p.barrier IS NOT NULL "
        "AND    ST_Intersects(p.way, e.geom)"
    );
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


