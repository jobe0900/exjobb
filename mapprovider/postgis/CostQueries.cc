/*
 * CostQueries.cc
 *
 *      Author: Jonas Bergman
 */


#include "CostQueries.h"  // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
void
CostQueries::getTravelTimeEdgeCosts(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rTopoEdgeTable,
    const std::string&      rOsmEdgeTable,
    const std::string&      rSchemaName)
{
    std::vector<std::string> columns {
        "maxspeed",
        "surface"
    };

    rResult = rTrans.exec(
        startOfQuery(rTopoEdgeTable) +
        queryColumns(columns) +
        midOfQuery(rSchemaName, rOsmEdgeTable) +
//        notNullColumns(columns) +
        endOfQuery()
    );
}

void
CostQueries::getOtherCosts(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rOsmPointTable,
    const std::string&      rTopoEdgeTable)
{
    rResult = rTrans.exec(
        "SELECT p.osm_id, "
        "       p.highway, "
        "       p.railway, "
        "       p.public_transport, "
        "       p.traffic_calming, "
        "       e.edge_id "
        "FROM  " + rOsmPointTable + " p, "
        "      " + rTopoEdgeTable + " e "
        "WHERE  (p.highway = 'bus_stop' OR "
        "       p.highway = 'crossing' OR "
        "       p.highway = 'give_way' OR"
        "       p.highway = 'mini_roundabout' OR"
        "       p.highway = 'stop' OR"
        "       p.highway = 'traffic_signals' OR"
        "       p.railway = 'level_crossing' OR"
        "       p.public_transport = 'stop_position' OR"
        "       p.traffic_calming = 'yes' OR"
        "       p.traffic_calming = 'bump' OR"
        "       p.traffic_calming = 'hump' OR"
        "       p.traffic_calming = 'table' OR"
        "       p.traffic_calming = 'cushion' OR"
        "       p.traffic_calming = 'rumble_strip' OR"
        "       p.traffic_calming = 'chicane' OR"
        "       p.traffic_calming = 'choker' OR"
        "       p.traffic_calming = 'island' "
        ")"
        "AND    ST_Intersects(p.way, e.geom)"
    );
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////
//static
std::string
CostQueries::startOfQuery(const std::string& rTopoEdgeTable)
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
CostQueries::queryColumns(const std::vector<std::string>& rCols)
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
CostQueries::midOfQuery(
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
CostQueries::notNullColumns(const std::vector<std::string>& rCols)
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
CostQueries::endOfQuery()
{
    return (
        ") AS osm "
        "ON edge_id = element_id "
        "ORDER BY edge_id ASC;"
    );
}


