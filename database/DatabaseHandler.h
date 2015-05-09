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

// PROJECT INCLUDES
//
#include <pqxx/pqxx>	// link with -lpqxx -lpq

// LOCAL INCLUDES
//
#include "../config/DatabaseConfig.h"
#include "DatabaseException.h"

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

	/** Build a PostGIS topoplogy.
	 * @param	rTopoName	Name to use for temporary tables and topo schema.
	 * @param	srid		The SRID for the projection to use
	 * @param	tolerance	The distance to look for merging vertices, unit of srid.
	 * @throws	DatabaseException
	 */
	void		buildTopology(const std::string& rTopoName,
							  int srid,
							  double tolerance);

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
	// ATTRIBUTES
	DatabaseConfig	mDbConfig;
	pqxx::connection mConnection;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* DATABASE_DATABASEHANDLER_H_ */
