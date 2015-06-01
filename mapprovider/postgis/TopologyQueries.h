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
