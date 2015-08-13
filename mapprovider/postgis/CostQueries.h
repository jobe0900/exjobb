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

    struct OtherCostResult
    {
        enum Columns
        {
            OSM_ID,
            HIGHWAY,
            RAILWAY,
            PUBLIC_TRANSPORT,
            TRAFFIC_CALMING,
            EDGE_ID
        };
    };

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

    /** Query for costs under the highway and railway tags:
     * Highway:
     *  - bus_stop
     *  - crossing
     *  - give_way
     *  - mini_roundabout
     *  - stop
     *  - traffic_signals
     * Railway:
     *  - level_crossing
     * Traffic calming
     *
     * @param   rTrans          Transaction to perform query in
     * @param   rResult         Store the result of the query here
     * @param   rOsmPointTable  Name of table with OSM points (nodes)
     * @param   rTopoEdgeTable  Name of table with topology edges.
     */
    static void             getOtherCosts(
        pqxx::transaction_base&     rTrans,
        pqxx::result&               rResult,
        const std::string&          rOsmPointTable,
        const std::string&          rTopoEdgeTable);

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
