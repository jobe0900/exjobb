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

// FORWARD REFERENCES
//

struct DatabaseConfig
{
	std::string		mHostname;
	int				mPort;
	std::string		mUsername;
	std::string		mPassword;
	std::string		mDatabase;

	// OPERATIONS
	/** Construct a connection string from the attributes.
	 * @return	A valid connection string for 'pqxx::conn()'
	 */
	std::string
	getConnectionString() {
		std::ostringstream oss;
		oss << "host=" << mHostname
			<< " port=" << mPort
			<< " user=" << mUsername
			<< " password=" << mPassword
			<< " dbname=" << mDatabase;
		return oss.str();
	}
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_DATABASECONFIG_H_ */
