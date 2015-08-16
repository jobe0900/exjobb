/*
 * TopologyQueries.cc
 *
 *  Created on: 2015-06-01
 *      Author: Jonas Bergman
 */

#include "TopologyQueries.h"  // class implemented

#include "../../osm/OsmHighway.h"


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

// static
void
TopologyQueries::addVertexResultToTopology(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    for(size_t row = 0; row < rResult.size(); ++row)
    {
        VertexIdType id(rResult[row][VertexResult::NODE_ID].as<int>());
        Point point(rResult[row][VertexResult::X].as<double>(),
                    rResult[row][VertexResult::Y].as<double>());
        rTopology.addVertex(id, point);
    }
}

//static
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

// static
void
TopologyQueries::addEdgeResultToTopology(
    const pqxx::result& rResult,
    Topology&           rTopology)
{
    for(const pqxx::tuple& row : rResult)
    {
        Edge& edge = addBasicResultToEdge(row, rTopology);
        addGeomDataResultToEdge(edge, row);
        addRoadDataResultToEdge(edge, row);
    }
}

// static
Edge&
TopologyQueries::addBasicResultToEdge(
    const pqxx::tuple&  rRow,
    Topology&           rTopology)
{
    EdgeIdType
        edge_id(rRow[EdgeResult::EDGE_ID].as<EdgeIdType>(Edge::MAX_ID));
    OsmIdType
        osm_id(rRow[EdgeResult::OSM_ID].as<OsmIdType>(Osm::MAX_ID));
    VertexIdType
        source_id(rRow[EdgeResult::START_NODE].as<int>(Vertex::MAX_ID));
    VertexIdType
        target_id(rRow[EdgeResult::END_NODE].as<int>(Vertex::MAX_ID));

    Edge& edge = rTopology.addEdge(edge_id, osm_id, source_id, target_id);

    return edge;
}

// static
void
TopologyQueries::addGeomDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::GeomData gd(
           rRow[EdgeResult::EDGE_LENGTH].as<double>(0),
           Point(rRow[EdgeResult::CENTER_X].as<double>(0),
                 rRow[EdgeResult::CENTER_Y].as<double>(0)),
           rRow[EdgeResult::SOURCE_BEARING].as<int>(0),
           rRow[EdgeResult::TARGET_BEARING].as<int>(0));
    rEdge.setGeomData(gd);
}

// static
void
TopologyQueries::addRoadDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow)
{
    Edge::RoadData rd;
    std::string
        onewayStr(rRow[EdgeResult::ONEWAY].as<std::string>("no"));

    if(rRow[EdgeResult::JUNCTION].as<std::string>("") ==
        OsmHighway::JUNCTION_ROUNDABOUT)
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

    rd.nrLanes = rRow[EdgeResult::LANES].as<size_t>(1);

    addHighwayTypeToEdgeRoadData(rd, rRow);

    rEdge.setRoadData(rd);
}

// static
void
TopologyQueries::addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                              const pqxx::tuple& rRow)
{
    std::string roadTypeStr( rRow[EdgeResult::HIGHWAY].as<std::string>("road"));
    try
    {
        rRoadData.roadType = OsmHighway::parseString(roadTypeStr);
    }
    catch (OsmException& oe)
    {
        throw MapProviderException(
            std::string("TopologyQueries:addHighwayTypeToEdgeRoadData:")
            + oe.what());
    }
}

//static
void
TopologyQueries::installPostgisTopology(pqxx::transaction_base& rTrans)
{
    rTrans.exec(
            "CREATE EXTENSION IF NOT EXISTS postgis_topology"
    );
}

//static
void
TopologyQueries::setSearchPath(pqxx::transaction_base& rTrans)
{
    rTrans.exec(
            "SET search_path = topology, public"
    );
}

//static
void
TopologyQueries::createTemporaryTable(pqxx::transaction_base& rTrans,
                                      const std::string& rTableName)
{
    rTrans.exec(
            "CREATE TABLE public." + rTableName + " " +
            "AS SELECT * "
            "FROM planet_osm_line "
            "WHERE highway IS NOT NULL"
    );
}

//static
void
TopologyQueries::createTemporarySchema(pqxx::transaction_base& rTrans,
                                      const std::string& rSchemaName, int srid)
{
    rTrans.exec(
            "SELECT topology.CreateTopology('" +
            rSchemaName + "'," +
            rTrans.quote(srid) + ")"
    );
}

//static
void
TopologyQueries::addTopoGeometryColumn(pqxx::transaction_base& rTrans,
                                       const std::string& rSchemaName,
                                       const std::string& rOsmEdgeTable)
{
    rTrans.exec(
            "SELECT topology.AddTopoGeometryColumn('" +
            rSchemaName + "', " +
            "'public', '" +
            rOsmEdgeTable + "', " +
            "'topo_geom', 'LINESTRING')"
    );
}

//static
void
TopologyQueries::fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
                                        const std::string& rSchemaName,
                                        const std::string& rOsmEdgeTable,
                                        double tolerance)
{
    rTrans.exec(
            "UPDATE public." +
            rOsmEdgeTable + " " +
            "SET topo_geom = topology.toTopoGeom(way, '" +
            rSchemaName +
            "', 1, " +
            rTrans.quote(tolerance) + ")"
    );
}

//static
void
TopologyQueries::dropTemporaryTable(pqxx::transaction_base& rTrans,
                                    const std::string& rTableName)
{
    rTrans.exec(
            "DROP TABLE IF EXISTS public." + rTableName
    );
}

//static
void
TopologyQueries::dropTemporarySchema(pqxx::transaction_base& rTrans,
                                     const std::string& rSchemaName)
{
    rTrans.exec(
            "DROP SCHEMA IF EXISTS " + rSchemaName + " CASCADE"
    );
}

//static
void
TopologyQueries::deleteTemporaryLayerRecord(pqxx::transaction_base& rTrans,
                                             const std::string& rTableName)
{
    rTrans.exec(
            "DELETE FROM topology.layer "
            "WHERE table_name = " + rTrans.quote(rTableName)
    );
}

//static
void
TopologyQueries::deleteTemporaryTopoRecord(pqxx::transaction_base& rTrans,
                                             const std::string& rSchemaName)
{
    rTrans.exec(
            "DELETE FROM topology.topology "
            "WHERE name = " + rTrans.quote(rSchemaName)
    );
}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




