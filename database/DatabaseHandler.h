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
	 * The provided configuration is used for all connections.
	 *
	 * @param	rDatabaseConfig		The configuration fo connections
	 */
	DatabaseHandler(const DatabaseConfig& rDatabaseConfig);


	// OPERATORS
	// OPERATIONS

	/** Get the version of the database.
	 * Used for testing connection.
	 *
	 * @return	A string representing the version.
	 */
	std::string	getDatabaseVersion();

	/** Make sure the postgis_topology extension is installed.
	 */
	void		installPostgisTopology();

	// ACCESS
	// INQUIRY

protected:

private:
	/** Default constructor.
	 * Hidden.
	 */
	DatabaseHandler();

	// ATTRIBUTES
	DatabaseConfig	mDbConfig;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* DATABASE_DATABASEHANDLER_H_ */
