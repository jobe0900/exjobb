/**  Queries for PostGisProvider to find costs.
 *
 * #include "CostQueries.h"
 *
 * @author Jonas Bergman
 */
#ifndef MAPPROVIDER_POSTGIS_COSTQUERIES_H_
#define MAPPROVIDER_POSTGIS_COSTQUERIES_H_

// SYSTEM INCLUDES
//
#include <string>
#include <sstream>
#include <vector>

// PROJECT INCLUDES
//
#include <boost/algorithm/string.hpp>
#include <pqxx/pqxx>

// LOCAL INCLUDES
//
#include "../../osm/OsmHighway.h"
#include "../../graph/Edge.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"
#include "../MapProviderException.h"

/** Class for holding static queries about costs,
 * needed by the PostGisProvider.
 */
class CostQueries
{
public:
// TYPES

    struct TravelTimeCostResult
    {
        enum Columns
        {
            EDGE_ID,
            ELEMENT_ID,
            MAXSPEED,
            SURFACE
        };
    };
//
//    /** Results from queries are handled by these functions. */
//    struct Results
//    {
//        /** Parse a row in the results from turning restrictions.
//         * @param   rRow        The row with results.
//         * @param   rTopology   The topology that needs to be queried.
//         * @return  A turning restriction object.
//         * @throw   pqxx::pqxx_exception
//         * @throw   TopologyException
//         * @throw   MapProviderException
//         */
//        static OsmTurningRestriction*
//                            parseTurningRestrictionResultRow(
//            const pqxx::tuple&    rRow,
//            Topology&             rTopology);
//
//
//        /** Try to parse the column 'edge_ids' from the 'turning_restrictions'.
//         * Split the string of edge ids and convert them to a vector of EdgeIds.
//         * @param   rEdgeIds    A string like "{123, 456}". Gets trimmed of {}.
//         * @return  A vector of the EdgeIds separately.
//         * @throw   std::invalid_argument
//         * @throw   std::out_of_range
//         */
//        static std::vector<EdgeIdType>
//                            parseEdgeIdsString(std::string& rEdgeIds);
//
//        /** Find the Edge that matches the OsmId in turning restriction.
//         * @param   osmId       The original edge osm id.
//         * @param   rEdgeIds    The candidate edges that are near restriction.
//         * @param   rTopology   The Topology to ask for edges.
//         */
//        static Edge&        findEdgeMatchingOsmId(
//            OsmIdType                       osmId,
//            const std::vector<EdgeIdType>&  rEdgeIds,
//            Topology&                       rTopology);
//    };

// LIFECYCLE
    CostQueries() = delete;
    CostQueries(const CostQueries& from) = delete;
    ~CostQueries() = default;

// OPERATORS
// OPERATIONS
    /** Query for costs for travel time, meaning we need to find the speed.
     * @param   rTrans          Transaction to perform query in.
     * @param   rResult         Store the result of query here.
     * @param   rTopoEdgeTable  Name of table with topology edges.
     * @param   rOsmEdgeTable   Name of table with OSM edges.
     * @param   rSchemaName     Name of the topology schema.
     * @throw   pqxx::pqxx_exception
     */
    static void             getTravelTimeEdgeCosts(
        pqxx::transaction_base& rTrans,
        pqxx::result&           rResult,
        const std::string&      rTopoEdgeTable,
        const std::string&      rOsmEdgeTable,
        const std::string&      rSchemaName);

// ACCESS
// INQUIRY

protected:
private:
    /** SELECT FROM JOIN */
    static std::string  startOfQuery(const std::string& rTopoEdgeTable);
    /** Which columns to pick */
    static std::string  queryColumns(const std::vector<std::string>& rCols);
    /** FROM JOIN ON WHERE */
    static std::string  midOfQuery(
        const std::string& rSchemaName,
        const std::string& rOsmEdgeTable);
    /** Make sure only to pick rows with content in some column. */
    static std::string  notNullColumns(const std::vector<std::string>& rCols);
    /** AS ON ORDER BY */
    static std::string  endOfQuery();
};

#endif /* MAPPROVIDER_POSTGIS_COSTQUERIES_H_ */
