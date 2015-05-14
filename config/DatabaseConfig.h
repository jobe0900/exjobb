/**  Data structure for configuration of database connnection.
 *
 * #include "DatabaseConfig.h"
 *
 * A longer description.
 *
 * @see something
 * @author Jonas Bergman
 */
#ifndef CONFIG_DATABASECONFIG_H_
#define CONFIG_DATABASECONFIG_H_

// SYSTEM INCLUDES
//
#include <string>
#include <sstream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "TopologyConfig.h"

// FORWARD REFERENCES
//

struct DatabaseConfig
{
// ATTRIBUTES
	std::string		hostname;
	int				port;
	std::string		username;
	std::string		password;
	std::string		database;


// OPERATIONS
	/** Construct a connection string from the attributes.
	 * @return	A valid connection string for 'pqxx::conn()'
	 */
	std::string
	getConnectionString() {
		std::ostringstream oss;
		oss << "host=" << hostname
			<< " port=" << port
			<< " user=" << username
			<< " password=" << password
			<< " dbname=" << database;
		return oss.str();
	}
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_DATABASECONFIG_H_ */
