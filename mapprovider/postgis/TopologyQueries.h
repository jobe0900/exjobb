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

// FORWARD REFERENCES
//


class TopologyQueries
{
public:
// TYPES
    struct VertexResult
    {
        enum Columns
        {
            NODE_ID,
            X,
            Y
        };
    };

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

            // vehicle property

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
    struct VehiclePropertiesResult
    {
        enum Columns
        {
            EDGE_ID,
            ELEMENT_ID,
            MAXHEIGHT,
            MAXLENGTH,
            MAXWEIGHT,
            MAXWIDTH,
            MAXSPEED,
            MINSPEED,

        };
    };

    struct AccessResult
    {
        enum Columns
        {
            EDGE_ID,
            ELEMENT_ID,
            ACCESS,
            BARRIER,
            DISUSED,
            NOEXIT,
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
     * @param   rVertexTable    Name of table to fetch vertices from.
     */
    static void     getTopologyVertices(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rVertexTable);

    /** Fetch the edges for the topology.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rTopoEdgeTable  Name of table to topology edges from.
     * @param   rSchemaName     Name of topology schema.
     * @param   rOsmEdgeTable   Name of table with original OSM edge data.
     */
    static void     getTopologyEdges(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rTopoEdgeTable,
        const std::string&      rSchemaName,
        const std::string&      rOsmEdgeTable);

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
