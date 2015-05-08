/**  Handle connections with database.
 *
 * #include "DatabaseHandler.h"
 *
 * @author Jonas Bergman
 */

#ifndef OSM_PROVIDER_POSTGISOSMPROVIDER_H_
#define OSM_PROVIDER_POSTGISOSMPROVIDER_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//
#include <pqxx/pqxx>	// link with -lpqxx -lpq

// LOCAL INCLUDES
//
#include "../config/DatabaseConfig.h"
#include "OsmProviderException.h"

// FORWARD REFERENCES
//

/**
 * A class to handle the reading of data from a json configuration file.
 */
class PostgisOsmProvider
{
public:
	// LIFECYCLE

	/** Constructor.
	 * The provided configuration is used for all connections.
	 *
	 * @param	rDatabaseConfig		The configuration fo connections
	 */
	PostgisOsmProvider(const DatabaseConfig& rDatabaseConfig);


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
	PostgisOsmProvider();

	// ATTRIBUTES
	DatabaseConfig	mDbConfig;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_PROVIDER_POSTGISOSMPROVIDER_H_ */
