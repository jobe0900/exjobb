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
#include <string>
#include <vector>

// PROJECT INCLUDES
//
#include <pqxx/pqxx>	// link with -lpqxx -lpq

// LOCAL INCLUDES
//
#include "../../config/DatabaseConfig.h"
#include "../../graph/Edge.h"
#include "../../graph/OsmConstants.h"
#include "../../graph/Topology.h"
#include "../../graph/Vertex.h"
#include "../MapProvider.h"
#include "../MapProviderException.h"
#include "../../util/TimeToStringMaker.h"

// FORWARD REFERENCES
//

/**
 * A class to handle the reading of data from the PostGis database.
 * The configurations for the connection and the base name of the
 * topology name is given.
 * from a json configuration file.
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

// INQUIRY

protected:

private:
// HELPERS
    // Helpers for 'getTopology()'
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

    /** Check if edge has restrictions on properties of the vehicle.
     * @param   rRow    Row with data for an Edge.
     * @return  True if vehicle can pass, false if not.
     */
    bool    validDimensionRestrictions(const pqxx::tuple& rRow);

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
     */
    void    addHighwayTypeToEdgeRoadData(Edge::RoadData& rRoadData,
                                         const pqxx::tuple& rRow);

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

    // Helpers for 'buildTopology()'
    void	installPostgisTopology(pqxx::transaction_base& rTrans);
    void	setSearchPath(pqxx::transaction_base& rTrans);
    void	createTemporaryTable(pqxx::transaction_base& rTrans,
                                 const std::string& rTableName);
    void	createTemporarySchema(pqxx::transaction_base& rTrans,
                                  const std::string& rSchemaName, int srid);
    void	addTopoGeometryColumn(pqxx::transaction_base& rTrans,
                                  const std::string& rSchemaName,
                                  const std::string& rTableName);
    void	fillTopoGeometryColumn(pqxx::transaction_base& rTrans,
                                   const std::string& rSchemaName,
                                   const std::string& rTableName,
                                   double tolerance);

    // Helpers for 'removeTopology()'
    void	dropTemporaryTable(pqxx::transaction_base& rTrans,
                               const std::string& rTableName);
    void	dropTemporarySchema(pqxx::transaction_base& rTrans,
                                const std::string& rSchemaName);
    void	deleteTemporaryLayerRecord(pqxx::transaction_base& rTrans,
                                       const std::string& rTableName);
    void	deleteTemporaryTopoRecord(pqxx::transaction_base& rTrans,
                                      const std::string& rSchemaName);

    // ATTRIBUTES
    const DatabaseConfig&   mDbConfig;
    const TopologyConfig&   mTopoConfig;
    pqxx::connection        mConnection;
    std::string             mTableName;
    std::string             mSchemaName;

    // CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_ */
