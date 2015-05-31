/*
 * PostGisRestrictionQueries.cc
 *
 *  Created on: 2015-05-31
 *      Author: Jonas Bergman
 */


#include "PostGisRestrictionQueries.h"  // class implemented

// Result --------------------------------------------------------------------
//static
OsmTurningRestriction
PostGisRestrictionQueries::Results::parseTurningRestrictionResultRow(
    const pqxx::tuple&    rRow,
    Topology&             rTopology)
{
    OsmIdType fromOsmId =
        rRow[PostGisRestrictionQueries::FROM_OSM_ID].as<OsmIdType>();
    OsmIdType toOsmId =
        rRow[PostGisRestrictionQueries::TO_OSM_ID].as<OsmIdType>();
    std::string typeString =
        rRow[PostGisRestrictionQueries::RESTRICTION_TYPE].as<std::string>();
    OsmTurningRestriction::TurningRestrictionType type =
        OsmTurningRestriction::parseString(typeString);
    std::string edgeIdsString =
        rRow[PostGisRestrictionQueries::EDGE_IDS].as<std::string>();
    std::string viaOsmIdsString =
        rRow[PostGisRestrictionQueries::VIA_OSM].as<std::string>("");

    std::vector<EdgeIdType> edgeIds = parseEdgeIdsString(edgeIdsString);

    Edge& fromEdge = findEdgeMatchingOsmId(fromOsmId, edgeIds, rTopology);
    Edge& toEdge   = findEdgeMatchingOsmId(toOsmId, edgeIds, rTopology);


    // VIA WAY
    if(fromEdge.target() != toEdge.source())
    {
        fromEdge.setHasRestrictions(true);
        fromEdge.setHasViaWayRestriction(true);
        OsmTurningRestriction restriction(
            type,
            fromEdge.id(),
            viaOsmIdsString,
            toEdge.id());
        return restriction;
    }
    // VIA NODE
    else
    {
        VertexIdType vertexId = fromEdge.target();
        fromEdge.setHasRestrictions(true);
        OsmTurningRestriction restriction(
            type,
            fromEdge.id(),
            vertexId,
            toEdge.id());
        return restriction;
    }
}

//static
std::vector<EdgeIdType>
PostGisRestrictionQueries::Results::parseEdgeIdsString(std::string& rEdgeIds)
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
PostGisRestrictionQueries::Results::findEdgeMatchingOsmId(
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
pqxx::result
PostGisRestrictionQueries::getVehiclePropertyEdgeRestrictions(
    pqxx::transaction_base& rTrans,
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

    return rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
        notNullColumns(columns) +
        endOfQuery()
    );
}

//static
pqxx::result
PostGisRestrictionQueries::getAccessRestrictions(
    pqxx::transaction_base& rTrans,
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

    return rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
        notNullColumns(columns) +
        endOfQuery()
    );
}

//static
void
PostGisRestrictionQueries::dropCreateTurningRestrictionsTable(
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
PostGisRestrictionQueries::identifyTurningRestrictions(
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
pqxx::result
PostGisRestrictionQueries::getTurningRestrictions(
    pqxx::transaction_base& rTrans)
{
    return rTrans.exec(
        "SELECT * FROM turning_restrictions"
    );
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////
//static
std::string
PostGisRestrictionQueries::startOfQuery(const std::string& rTopoEdgeTable)
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
PostGisRestrictionQueries::queryColumns(const std::vector<std::string>& rCols)
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
PostGisRestrictionQueries::midOfQuery(
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
PostGisRestrictionQueries::notNullColumns(const std::vector<std::string>& rCols)
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
PostGisRestrictionQueries::endOfQuery()
{
    return (
        ") AS osm "
        "ON edge_id = element_id "
        "ORDER BY edge_id ASC;"
    );
}


