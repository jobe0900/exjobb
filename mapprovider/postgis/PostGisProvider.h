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
#include "../../graph/TopologyVertex.h"
#include "../../graph/TopologyEdge.h"
#include "../../graph/Topology.h"
#include "../MapProvider.h"
#include "../MapProviderException.h"

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
     * @param   rTopoName               The name for the topology.
     * @param	rDatabaseConfig		    The configuration for connections.
     * @throws	MapProviderException    If connection could not be established.
     */
    PostGisProvider(const std::string&    rTopoName,
        const DatabaseConfig& rDatabaseConfig);

    /** Destructor.
     * Close connection to database
     */
    virtual ~PostGisProvider();


// OPERATORS
// OPERATIONS

    virtual void    getTopologyEdges(std::map<EdgeId, TopologyEdge>& rEdgeMap);
    virtual void    getTopologyVertices(std::map<VertexId, TopologyVertex>& rVertexMap);

    /** Build a PostGIS topology with name given in constructor.
     * @param	srid		The SRID for the projection to use
     * @param	tolerance	The distance to look for merging vertices, unit of srid.
     * @throws	MapProviderException
     */
    void	        buildTopology(int srid, double tolerance);

    /** Remove PostGIS topology (tables and schema) from the database.
     * @throws	MapProviderException
     */
    void	        removeTopology();

// ACCESS
// INQUIRY

protected:

private:
// HELPERS
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
    const std::string   mTopoName;
    DatabaseConfig      mDbConfig;
    pqxx::connection    mConnection;
    std::string         mTableName;
    std::string         mSchemaName;

    // CONSTANTS
    const std::string TEMP_SCHEMA_PREFIX = "topo_";
    const std::string TEMP_TABLE_PREFIX = "highways_";
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_POSTGISPROVIDER_H_ */