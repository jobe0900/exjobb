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
     * @param   rVertexTable    Name of table to fetch vertices from.
     * @return  pqxx::result
     */
    static pqxx::result    getTopologyVertices(
        pqxx::transaction_base& rTrans,
        const std::string& rVertexTable);

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
