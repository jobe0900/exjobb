/**  Handle connections with database.
 *
 * #include "DatabaseHandler.h"
 *
 * @author Jonas Bergman
 */

#ifndef DATABASE_DATABASEHANDLER_H_
#define DATABASE_DATABASEHANDLER_H_

// SYSTEM INCLUDES
//
#include <string>
#include <vector>

// PROJECT INCLUDES
//
#include <pqxx/pqxx>	// link with -lpqxx -lpq

// LOCAL INCLUDES
//
#include "../config/DatabaseConfig.h"
#include "DatabaseException.h"
#include "../graph/TopologyVertex.h"

// FORWARD REFERENCES
//

/**
 * A class to handle the reading of data from a json configuration file.
 */
class DatabaseHandler
{
public:
	// LIFECYCLE

	/** Constructor.
	 * Establish connection to database.
	 *
	 * @param	rDatabaseConfig		The configuration for connections
	 * @throws	DatabaseException	If connection could not be established.
	 */
	DatabaseHandler(const DatabaseConfig& rDatabaseConfig);

	/** Destructor.
	 * Close connection to database
	 */
	~DatabaseHandler();


	// OPERATORS
	// OPERATIONS

	/** Build a PostGIS topology.
	 * @param	rTopoName	Name to use for temporary tables and topo schema.
	 * @param	srid		The SRID for the projection to use
	 * @param	tolerance	The distance to look for merging vertices, unit of srid.
	 * @throws	DatabaseException
	 */
	void	buildTopology(const std::string& rTopoName,
							  int srid,
							  double tolerance);

	/** Remove a PostGIS topology (tables and schema) from the database.
	 * @param	rTopoName	Name base used when creating topo table and schema.
	 * @throws	DatabaseException
	 */
	void	removeTopology(const std::string& rTopoName);

	/** Get topology vertices from the db and put them in the vector.
	 * @param	rTopoName			Name for the topology to query.
	 * @param	rTopologyVertices	Vector to store the vertices in.
	 * @throws	DatabaseException
	 */
	void	getTopologyVertices(const std::string& rTopoName,
								std::vector<TopologyVertex*>& rTopologyVertices);

	// ACCESS
	// INQUIRY

protected:

private:
	/** Default constructor.
	 * Hidden.
	 */
	DatabaseHandler();

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
	DatabaseConfig	mDbConfig;
	pqxx::connection mConnection;

	// CONSTANTS
	const std::string TEMP_SCHEMA_PREFIX = "topo_";
	const std::string TEMP_TABLE_PREFIX = "highways_";
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* DATABASE_DATABASEHANDLER_H_ */
