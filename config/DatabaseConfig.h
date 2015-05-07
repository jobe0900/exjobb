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
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_DATABASECONFIG_H_ */
