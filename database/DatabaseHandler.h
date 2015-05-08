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
#include "DatabaseConfig.h"

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
	 * @param	rDatabaseConfig		The configuration structure to populate.
	 */
	std::string	getDatabaseVersion();

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
