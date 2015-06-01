/*
 * TopologyQueries.cc
 *
 *  Created on: 2015-06-01
 *      Author: Jonas Bergman
 */

#include "TopologyQueries.h"  // class implemented

#include "../../osm/OsmHighway.h"


/////////////////////////////// TYPES  ///////////////////////////////////////
// Result---------------------------------------------------------------------


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
void
TopologyQueries::getTopologyVertices(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rVertexTable)
{
		rResult = rTrans.exec(
				"SELECT node_id, ST_X(geom) AS x, ST_Y(geom) AS y "
				" FROM " + rVertexTable +
				" ORDER BY node_id ASC;"
		);
}

void
TopologyQueries::getTopologyEdges(
    pqxx::transaction_base& rTrans,
    pqxx::result&           rResult,
    const std::string&      rTopoEdgeTable,
    const std::string&      rSchemaName,
    const std::string&      rOsmEdgeTable)
{
    std::string sql(
        "SELECT     edge_id, "
        "           start_node, "
        "           end_node, "
        //-- geom data about edge
        "           ST_Length(geom) AS edge_length, "
        "           ST_X(ST_LineInterpolatePoint(geom, 0.5)) AS center_x, "
        "           ST_Y(ST_LineInterpolatePoint(geom, 0.5)) AS center_y, "
        "           (ST_Azimuth("
        "               ST_PointN(geom,1), "
        "               ST_PointN(geom,2))/(2*pi())*360)::int "
        "               AS source_bearing, "
        "           (ST_Azimuth("
        "               ST_PointN(geom,ST_NPoints(geom)-1), "
        "               ST_PointN(geom,ST_NPoints(geom)))/(2*pi())*360)::int "
        "               AS target_bearing, "
        //-- osm data about original edge
        "           osm.* "
        "FROM      " + rTopoEdgeTable +
        " JOIN ( "
        "   SELECT  osm_id, element_id "
        //-- road data
        "           , highway "
        "           , junction "
        "           , lanes "
        "           , oneway "
        "   FROM    " + rSchemaName + ".relation "
        "   JOIN    " + rOsmEdgeTable+
        "   ON      topogeo_id = (topo_geom).id "
        "   WHERE   highway in " + OsmHighway::typesAsCommaSeparatedString() +
        ") AS osm "
        "ON edge_id = element_id "
        "ORDER BY edge_id ASC;"
    );
    rResult = rTrans.exec(sql);
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




