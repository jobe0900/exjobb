/**  A class for holding static queries about the topology for the
 * PostGisProvider.
 *
 * #include "TopologyQueries.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_TOPOLOGYQUERIES_H_
#define MAPPROVIDER_POSTGIS_TOPOLOGYQUERIES_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//
#include <pqxx/pqxx>

// LOCAL INCLUDES
//
#include "../../graph/Topology.h"
#include "../../graph/TopologyException.h"
#include "../MapProviderException.h"

// FORWARD REFERENCES
//

/** This class holds static queries about the topology to be used by the
 * PostGisProvider. It also holds types for handling the results.
 */
class TopologyQueries
{
public:
// TYPES

    /** Columns used in queries for Vertices. */
    struct VertexResult
    {
        enum Columns
        {
            NODE_ID,
            X,
            Y
        };
    };

    /** Columns used in queries for Edges. */
    struct EdgeResult
    {
        enum Columns
        {
            EDGE_ID,
            START_NODE,
            END_NODE,
            EDGE_LENGTH,
            CENTER_X,
            CENTER_Y,
            SOURCE_BEARING,
            TARGET_BEARING,
            OSM_ID,
            ELEMENT_ID,     // NOT USED: same as EDGE_ID_COL

            // road data
            HIGHWAY,
            JUNCTION,
            LANES,
            ONEWAY,

            // access
            ACCESS,
            MOTORCAR,
            GOODS,
            HGV,
            PSV,
            LHV,
            MOTOR_VEHICLE,
            VEHICLE,
        };
    };



// LIFECYCLE
    /** Constructor. */
    TopologyQueries() = delete;
    /** Copy constructor. */
    TopologyQueries(const TopologyQueries& from) = delete;

// OPERATORS
// OPERATIONS
    /** Fetch the vertices for the topology.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rVertexTable    Name of table to fetch topology vertices from.
     */
    static void     getTopologyVertices(
                        pqxx::transaction_base& rTrans,
                        pqxx::result&           rResult,
                        const std::string&      rVertexTable);

    /** Add vertices to topology.
     * @throws  TopologyException
     */
    static void     addVertexResultToTopology(
                        const pqxx::result&     rResult,
                        Topology&               rTopology);

    /** Fetch the edges for the topology.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rTopoEdgeTable  Name of table to fetch topology edges from.
     * @param   rSchemaName     Name of topology schema.
     * @param   rOsmEdgeTable   Name of table with original OSM edge data.
     */
    static void     getTopologyEdges(
                        pqxx::transaction_base& rTrans,
                        pqxx::result&           rResult,
                        const std::string&      rTopoEdgeTable,
                        const std::string&      rSchemaName,
                        const std::string&      rOsmEdgeTable);

    /** Add edges to topology.
     * @param   rEdgeResult    Result of db query for edges.
     * @param   rTopology      Topology to fill with edges.
     * @throws  TopologyException
     */
    static void     addEdgeResultToTopology(
                        const pqxx::result&     rResult,
                        Topology&               rTopology);

    /** Helper to add basic data from db to Edge.
     * @param   rRow        Row with data for an Edge.
     * @param   rTopology   Topology to add edge to.
     * @return  Reference to the newly added Edge.
     * @throws  TopologyException
     */
    static Edge&    addBasicResultToEdge(
                        const pqxx::tuple&  rRow,
                        Topology&           rTopology);

    /** Add geometric result from query to an Edge.
     * @param   rEdge   Reference to Edge to set Geom data on.
     * @param   rRow    Reference to Row with Geom data in it.
     */
    static void     addGeomDataResultToEdge(
                        Edge&               rEdge,
                        const pqxx::tuple&  rRow);

    /** Add road related result from query to an Edge.
     * @param   rEdge   Reference to Edge to set road data on.
     * @param   rRow    Reference to Row with road data in it.
     */
    static void     addRoadDataResultToEdge(
                        Edge&               rEdge,
                        const pqxx::tuple&  rRow);

    /** Extract highway type from database result and store in RoadData.
     * @param   rRoadData   The RoadData to store in.
     * @param   rRow    Reference to Row with road data in it.
     * @throw   MapProviderException
     */
    static void     addHighwayTypeToEdgeRoadData(
                        Edge::RoadData&     rRoadData,
                        const pqxx::tuple&  rRow);

    /** Make sure the 'postgis_topology' extension is installed.
     * @param   rTrans          Transaction to perform query in.
     */
    static void     installPostgisTopology(pqxx::transaction_base& rTrans);

    /** Set schema search path for queries.
     * @param   rTrans          Transaction to perform query in.
     */
    static void     setSearchPath(pqxx::transaction_base& rTrans);

    /** Create the temporary table for topologies.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTableName      Name of the temporary topology table.
     */
    static void     createTemporaryTable(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rTableName);

    /** Create a schema for the temporary postgis topology.
     * @param   rTrans          Transaction to perform query in.
     * @param   rSchemaName     Name of the temporary schema.
     * @param   srid            The projection to use.
     */
    static void     createTemporarySchema(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rSchemaName,
                        int                     srid);

    /** Add a column for geometry in the table with Osm Edges.
     * @param   rTrans          Transaction to perform query in.
     * @param   rSchemaName     Name of the temporary schema.
     * @param   rOsmeEdgeTable  Name of the table with OSM edges.
     */
    static void     addTopoGeometryColumn(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rSchemaName,
                        const std::string&      rOsmEdgeTable);

    /** Fill geometry in the table with Osm Edges, using a tolerance for
     * merging nodes near one another.
     * @param   rTrans          Transaction to perform query in.
     * @param   rSchemaName     Name of the temporary schema.
     * @param   rOsmeEdgeTable  Name of the table with OSM edges.
     * @param   tolerance       Tolerance in unit of projection.
     */
    static void     fillTopoGeometryColumn(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rSchemaName,
                        const std::string&      rOsmEdgeTable,
                        double                  tolerance);

    /** Drop the temporary table for topologies.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTableName      Name of the temporary topology table.
     */
    static void     dropTemporaryTable(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rTableName);

    /** Drop the temporary schema for topologies.
     * @param   rTrans          Transaction to perform query in.
     * @param   rSchemaName     Name of the temporary topology scheam.
     */
    static void     dropTemporarySchema(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rTableName);

    /** Clean up in records for postgis topologies.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTableName      Name of the temporary topology table.
     */
    static void     deleteTemporaryLayerRecord(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rTableName);

    /** Clean up in records for postgis topologies.
     * @param   rTrans          Transaction to perform query in.
     * @param   rTableName      Name of the temporary topology table.
     */
    static void     deleteTemporaryTopoRecord(
                        pqxx::transaction_base& rTrans,
                        const std::string&      rSchemaName);

// ACCESS
// INQUIRY

protected:
private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_TOPOLOGYQUERIES_H_ */
