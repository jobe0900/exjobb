/**  Handle connections with PostGis database to get map data.
 *
 * #include "PostGisProvider.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_
#define MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_

// SYSTEM INCLUDES
//
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

// PROJECT INCLUDES
//
#include <pqxx/pqxx>	// link with -lpqxx -lpq
#include <boost/algorithm/string.hpp>

// LOCAL INCLUDES
//
#include "CostQueries.h"
#include "RestrictionQueries.h"
#include "TopologyQueries.h"
#include "../MapProvider.h"
#include "../MapProviderException.h"
#include "../../config/DatabaseConfig.h"
#include "../../graph/Edge.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"
#include "../../graph/Speed.h"
#include "../../osm/OsmAccess.h"
#include "../../osm/OsmHighway.h"
#include "../../osm/OsmVehicle.h"
#include "../../util/TimeToStringMaker.h"
#include "LineGraphBuilderSaveQueries.h"

// FORWARD REFERENCES
//

/**
 * A class to handle the reading of data from the PostGis database.
 * The configurations for the connection and the base name of the
 * topology name is given.
 */
class PostGisProvider : public MapProvider
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    PostGisProvider() = delete;

    /** Constructor.
     * Establish connection to database.
     *
     * @param	rConfig		            The configuration.
     * @throws	MapProviderException    If connection could not be established.
     */
    PostGisProvider(const Configuration& rConfig);

    /** Destructor.
     * Close connection to database
     */
    virtual ~PostGisProvider();


// OPERATORS
// OPERATIONS
    virtual void    getTopology(Topology& rTopology);

    virtual void    setRestrictionsAndCosts(Topology& rTopology);

    virtual void    persistLineGraph(const GraphBuilder& rGraph);

// INQUIRY

protected:

private:
// HELPERS

    /** Get edges from database.
     * @param   rEdgeResult   Result of db query for Edges.
     * @throws	MapProviderException
     */
    void    getTopologyEdges(pqxx::result& rEdgeResult);

    /** Add edges to topology.
     * @param   rEdgeResult    Result of db query for edges.
     * @param   rTopology      Topology to fill with edges.
     * @throws  TopologyException
     */
    void    addEdgeResultToTopology(const pqxx::result& rEdgeResult,
                                    Topology& rTopology);

    /** Get vertices from database.
     * @throws	MapProviderException
     */
    void    getTopologyVertices(pqxx::result& rVertexResult);

    /** Add vertices to topology.
     * @throws  TopologyException
     */
    void    addVertexResultToTopology(
                const pqxx::result& rResult,
                Topology&           rTopology);

    // Helpers for constructor
    /** Set the base name for the topology, either a string from config
     * or a timestamp.
     */
    void    setTopoBaseName(std::string& rTopoBaseName);

    /** Build a PostGIS topology with name given in constructor.
     * @param	srid		The SRID for the projection to use
     * @param	tolerance	The distance to look for merging vertices, unit of srid.
     * @throws	MapProviderException
     */
    void	buildTopology(int srid, double tolerance);

    /** Remove PostGIS topology (tables and schema) from the database.
     * @throws	MapProviderException
     */
    void	removeTopology();


    // Restriction helpers ---------------------------------------------------

    /** Add restrictions to edges.
     * @param   rTopology  Adding EdgeRestricion to Edges in topology.
     * @throw   MapProviderException
     */
    void    addEdgeRestrictions(Topology& rTopology);

    /** Get VehicleProperty restrictions
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult     Store the result of query in here.
     * @throw   MapProviderException
     */
    void    getVehiclePropertyEdgeRestrictions(pqxx::result& rResult);

    /** Add the result of the query for vehicle properties to Edge's restrictions.
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult         The results of the query
     * @param   rTopology       Update affected edges in the topology.
     * @throw   MapProviderException
     */
    void    addVehiclePropertyRestrictionsToEdge(
                const pqxx::result&    rResult,
                Topology&              rTopology);

    /** Get Access restrictions to edge.
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult     Store the result of query in here.
     * @throw   MapProviderException
     */
    void    getAccessRestrictions(pqxx::result& rResult);

    /** Add the result of the query for Access to restrictions.
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult         The results of the query
     * @param   rTopology       Update affected edges in the topology.
     * @throw   MapProviderException
     */
    void    addAccessRestrictionsToEdge(
                const pqxx::result&    rResult,
                Topology&              rTopology);

    /** Get Turning restrictions for traveling from edge.
     * Helper for 'getEdgeRestrictions()'.
     * Turning restrictions are relations and not easily handled with
     * osm2pgsql. Therefore we must use 'slim' mode when converting OSM to
     * PostGis, and use the table 'planet_osm_rels' and column 'tags' to look
     * for a 'restriction'. If we find one we have to parse the 'members'
     * column ourselves.
     * @param   rResult     Store the result of query in here.
     * @throw   MapProviderException
     */
    void    getTurningRestrictions(pqxx::result& rResult);

    /** Add the result of the query for Turning restrictions.
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult         The results of the query
     * @param   rTopology       Update affected edges in the topology.
     * @throw   MapProviderException
     */
    void    addTurningRestrictionsToEdge(
                const pqxx::result&    rResult,
                Topology&              rTopology);

    /** Get restrictions defined at points but applicable to edges,
     * such as barriers and railway crossings.
     * @param   rResult     Store the result of query in here.
     * @throw   MapProviderException
     */
    void    getEdgePointRestrictions(pqxx::result& rResult);

    /** Add the result of the query for Point restrictions on Edges .
     * Helper for 'getEdgeRestrictions()'
     * @param   rResult         The results of the query
     * @param   rTopology       Update affected edges in the topology.
     * @throw   MapProviderException
     */
    void    addPointRestrictionsToEdge(
                const pqxx::result&    rResult,
                Topology&              rTopology);

    // Costs -----------------------------------------------------------------
    /** Add costs to the edge.
     * @param   The Topology with Edges to add cost to.
     */
    void    addEdgeCosts(Topology& rTopology);

    /** Get costs for the travel time along an edge.
     * The length is constant in the topology but we need to find out if
     * there is a max speed restriction or if there is a bad surface.
     * If no such restrictions are in the database then the default speed
     * for the road category is used.
     * @param   rResult     Store the result of the query here.
     * @throw   MapProviderException
     */
    void    getTravelTimeCosts(pqxx::result& rResult);

    /** Add costs for travel time along the edge.
     * First set the speed of those with explicit restrictions in database,
     * then set the default speed for those without explicit speeds.
     * @param   rResult     The results of the query.
     * @param   rTopology   The topology with edges to set cost for.
     * @throw   MapProviderException
     */
    void    addTravelTimeCosts(const pqxx::result& rResult, Topology& rTopology);

    /** Get other costs for the edge other than speed and barriers, those
     * include slowdown at stop and yield signs, zebra crossings, railway
     * crossings, bus stops, speed bumps, traffic lights...
     * @param   rResult     Store the result of the query here.
     */
    void    getOtherEdgeCosts(pqxx::result& rResult);

    /** Add costs for speed bumps and such to affected edges.
     * @param   rResult     The results of the query.
     * @param   rTopology   The topology with edges to set cost for.
     * @throw   MapProviderException
     */
    void    addOtherCosts(const pqxx::result& rResult, Topology& rTopology);

    // LineGraph persistence -------------------------------------------------
    /** Set up the schema and tables needed to persist the line graph.
     */
    void    setUpSchemaAndTables();

    /** Create a new schema in the database
     * @throw   MapProviderException
     */
    void    createLineGraphSchema();

    /** Create the needed tables in the database
     * @throw   MapProviderException
     */
    void    createLineGraphTables();

    /** Insert the data in the database.
     * @param   rGraph      The graph with data.
     * @throw   MapProviderException
     */
    void    insertData(const GraphBuilder& rGraph);

    /** Prepare the LineGraph data for inserting into the database.
     * @param   rTrans      The transaction to operate within.
     * @param   rGraph      The graph data
     * @throw   MapProviderException
     */
    void    prepareLineGraphData(
                pqxx::transaction_base& rTrans,
                const GraphBuilder&            rGraph);

    // Generic helpers to clean up the code some -----------------------------
    /** Check that the connection with the database is up, or throw exception.
     * @throw   MapProviderException
     */
    void    testConnection();

// ATTRIBUTES
    const Configuration&    mConfig;
    const DatabaseConfig&   mDbConfig;
    const TopologyConfig&   mTopoConfig;
    pqxx::connection        mConnection;
    std::string             mOsmEdgeTable;
    std::string             mPointTableName;
    std::string             mSchemaName;
    std::string             mTopoEdgeTable;
    std::string             mEdgeIdCol;
    std::string             mSourceCol;
    std::string             mTargetCol;
    std::string             mEdgeGeomCol;
    std::string             mTopoVertexTable;
    std::string             mVertexIdCol;
    std::string             mVertexGeomCol;
    std::string             mLineGraphSchema;
    std::string             mLineGraphNodeTable;
    std::string             mLineGraphLineTable;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_ */
