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
#include "../../graph/EdgeRestriction.h"
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
    static void     getTravelTimeEdgeCosts(
                        pqxx::transaction_base& rTrans,
                        pqxx::result&           rResult,
                        const std::string&      rTopoEdgeTable,
                        const std::string&      rOsmEdgeTable,
                        const std::string&      rSchemaName);

    /** Add costs for travel time along the edge.
     * First set the speed of those with explicit restrictions in database,
     * then set the default speed for those without explicit speeds.
     * @param   rResult     The results of the query.
     * @param   rTopology   The topology with edges to set cost for.
     * @param   rConfig         Configuration
     * @throw   MapProviderException
     */
    static void     addTravelTimeCosts(
                        const pqxx::result&     rResult,
                        Topology&               rTopology,
                        const Configuration&    rConfig);

    /** Add cost relating to the maxspeed of the edge.
     * The cost is the number of seconds to travel the edge.
     * @param   rEdge           The edge to add cost to
     * @param   speed           The speed for the edge found in the database.
     * @param   surfaceString   The surface as string or empty if not specified.
     * @param   rConfig         Configuration
     * @throw   MapProviderException
     */
    static void     addTravelTimeCostToEdge(
                        Edge&                   rEdge,
                        Speed                   speed,
                        std::string&            surfaceString,
                        const Configuration&    rConfig);

    /** If the speed in the db was not set we must fetch the default
     * for this road category from the configuration.
     * @param   rEdge   The edge to find the default speed for
     * @param   rConfig         Configuration
     * @return  The default speed for this type of highway.
     */
    static Speed    getDefaultSpeedForEdge(
                        const Edge&             rEdge,
                        const Configuration&    rConfig);

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
    static void     getOtherCosts(
                        pqxx::transaction_base&     rTrans,
                        pqxx::result&               rResult,
                        const std::string&          rOsmPointTable,
                        const std::string&          rTopoEdgeTable);

    /** Add costs for speed bumps and such to affected edges.
     * @param   rResult     The results of the query.
     * @param   rTopology   The topology with edges to set cost for.
     * @param   rConfig     The Configuration
     * @throw   MapProviderException
     */
    static void     addOtherCosts(
                        const pqxx::result&     rResult,
                        Topology&               rTopology,
                        const Configuration&    rConfig);

    /** Add a cost of an other type to the edge.
     * Look up the value in the configuration.
     * @param   rEdge   The Edge to add a cost to.
     * @param   key     The type of cost as a string
     * @param   rConfig     The Configuration
     */
    static void     addOtherCostToEdge(
                        Edge&                   rEdge,
                        const std::string&      key,
                        const Configuration&    rConfig);

    /** While looking for restrictions and we come across barriers,
     * add the costs for barriers if they incur costs.
     * @param   edge    The edge with a barrier.
     * @param   type    The type of barrier.
     * @param   rConfig The Configuration to use for the cost.
     */
    static void     addBarrierCostToEdge(
                        Edge&                   rEdge,
                        OsmBarrier::BarrierType type,
                        const Configuration&    rConfig);

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
