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
#include "../../config/DatabaseConfig.h"
#include "../../graph/Edge.h"
#include "../../graph/Restrictions.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"
#include "../MapProvider.h"
#include "../MapProviderException.h"
#include "../../util/TimeToStringMaker.h"
#include "../../osm/OsmAccess.h"
#include "../../osm/OsmHighway.h"
#include "../../osm/OsmVehicle.h"
#include "RestrictionQueries.h"

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
//    virtual void    getMapData(Topology& rTopology);
    virtual void    getTopology(Topology& rTopology);

    virtual void    setRestrictionsAndCosts(Topology& rTopology);

//    virtual void    getRestrictions(
//        Restrictions&  rRestrictions,
//        Topology&      rTopology);

// INQUIRY

protected:

private:
// HELPERS
//    /** Get vertices from database.
//     * @param   rVertexResult   Result of db query for Vertices.
//     * @throws	MapProviderException
//     */
//    void    getVerticesFromDb(pqxx::result& rVertexResult);
//
//    /** Get edges from database.
//     * @param   rEdgeResult   Result of db query for Edges.
//     * @throws	MapProviderException
//     */
//    void    getEdgesFromDb(pqxx::result& rEdgeResult);


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

    /** Helper to add basic data from db to Edge.
     * @param   rRow        Row with data for an Edge.
     * @param   rTopology   Topology to add edge to.
     * @return  Reference to the newly added Edge.
     * @throws  TopologyException
     */
    Edge&   addBasicResultToEdge(const pqxx::tuple& rRow, Topology& rTopology);

    /** Add geometric result from query to an Edge.
     * @param   rEdge   Reference to Edge to set Geom data on.
     * @param   rRow    Reference to Row with Geom data in it.
     */
    void    addGeomDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow);

    /** Add road related result from query to an Edge.
     * @param   rEdge   Reference to Edge to set road data on.
     * @param   rRow    Reference to Row with road data in it.
     */
    void    addRoadDataResultToEdge(Edge& rEdge, const pqxx::tuple& rRow);

    /** Extract highway type from database result and store in RoadData.
     * @param   rRoadData   The RoadData to store in.
     * @param   rRow    Reference to Row with road data in it.
     * @throw   MapProviderException
     */
    void    addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                         const pqxx::tuple& rRow);

    /** Get the names of the highway columns we are interested in from OsmConst.
     * @return  String to put in query.
     */
    std::string getInterestingHighwayColumns() const;

    /** Get vertices from database.
     * @throws	MapProviderException
     */
    void    getTopologyVertices(pqxx::result& rVertexResult);

    /** Add vertices to topology.
     * @throws  TopologyException
     */
    void    addVertexResultToTopology(const pqxx::result&, Topology& rTopology);

    // Helpers for constructor
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
//    /** Add restrictions and costs to the edges of the topology.
//     * @param   rTopology  The Edges in topology to mark.
//     * @throw   MapProviderException
//     */
//    void    addRestrictionsAndCosts(Topology& rTopology);

    /** Add restrictions to edges.
     * @param   rTopology  Adding EdgeRestricion to Edges in topology.
     * @throw   MapProviderException
     */
    void    addEdgeRestrictions(Topology& rTopology);

//    /** Add costs to the edges.
//     * @param   rTopology  Add costs to the Edges in the Topology.
//     */
//    void    addEdgeCosts(Topology& rTopology);

//    /** Fetch Restrictions for edges.
//     * @param   rRestrictions   Store restrictions in here.
//     * @param   rTopology       Might get modified from turn restrictions.
//     * @throw   MapProviderException
//     */
//    void    getEdgeRestrictions(
//        Restrictions& rRestrictions,
//        Topology& rTopology);

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

//    /** Add the result of the query for vehicle properties to restrictions.
//     * Helper for 'getEdgeRestrictions()'
//     * @param   rResult         The results of the query
//     * @param   rRestrictions   Store the restrictions here.
//     * @param   rTopology       Update affected edges in the topology.
//     * @throw   MapProviderException
//     */
//    void    addVehiclePropertyResultToEdgeRestrictions(
//                const pqxx::result&    rResult,
//                Restrictions&          rRestrictions,
//                Topology&              rTopology);

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

//    /** Add the result of the query for Access to restrictions.
//     * Helper for 'getEdgeRestrictions()'
//     * @param   rResult         The results of the query
//     * @param   rRestrictions   Store the restrictions here.
//     * @param   rTopology       Update affected edges in the topology.
//     * @throw   MapProviderException
//     */
//    void    addAccessResultToEdgeRestrictions(
//                const pqxx::result&    rResult,
//                Restrictions&          rRestrictions,
//                Topology&              rTopology);

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

//    /** Add the result of the query for Turning to restrictions.
//     * Helper for 'getEdgeRestrictions()'
//     * @param   rResult         The results of the query
//     * @param   rRestrictions   Store the restrictions here.
//     * @param   rTopology       Update affected edges in the topology.
//     * @throw   MapProviderException
//     */
//    void    addTurningResultToEdgeRestrictions(
//                const pqxx::result&    rResult,
//                Restrictions&          rRestrictions,
//                Topology&              rTopology);

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

    /** Add cost relating to the maxspeed of the edge.
     * The cost is the number of seconds to travel the edge.
     * @param   rEdge           The edge to add cost to
     * @param   speed           The speed for the edge found in the database.
     * @param   surfaceString   The surface as string or empty if not specified.
     * @throw   MapProviderException
     */
    void    addTravelTimeCostToEdge(
                Edge& rEdge,
                Speed speed,
                std::string& surfaceString);

    /** If the speed in the db was not set we must fetch the default
     * for this road category from the configuration.
     * @param   rEdge   The edge to find the default speed for
     * @return  The default speed for this type of highway.
     */
    Speed   getDefaultSpeedForEdge(const Edge& rEdge) const;

//    /** Add the result of the query for Point restriciotns on Edges to Restrictions.
//     * Helper for 'getEdgeRestrictions()'
//     * @param   rResult         The results of the query
//     * @param   rRestrictions   Store the restrictions here.
//     * @param   rTopology       Update affected edges in the topology.
//     * @throw   MapProviderException
//     */
//    void    addPointResultToEdgeRestrictions(
//                const pqxx::result&    rResult,
//                Restrictions&          rRestrictions,
//                Topology&              rTopology);

// ATTRIBUTES
    const Configuration&    mConfig;
    const DatabaseConfig&   mDbConfig;
    const TopologyConfig&   mTopoConfig;
    pqxx::connection        mConnection;
//    pqxx::result            mEdgeResult;
//    pqxx::result            mVertexResult;
    std::string             mHighwayTableName;
    std::string             mPointTableName;
    std::string             mSchemaName;
    std::string             mEdgeTable;
    std::string             mEdgeIdCol;
    std::string             mSourceCol;
    std::string             mTargetCol;
    std::string             mEdgeGeomCol;
    std::string             mVertexTable;
    std::string             mVertexIdCol;
    std::string             mVertexGeomCol;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_ */
