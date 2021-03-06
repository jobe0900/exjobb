/**  Data structure for configuration of database connection.
 *
 * #include "DatabaseConfig.h"
 *
 * @author  Jonas Bergman
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

/** A simple data structure for holding the configuration
 * for database connections.
 */
struct DatabaseConfig
{
// ATTRIBUTES
    std::string        hostname;
    int                port;
    std::string        username;
    std::string        password;
    std::string        database;


// OPERATIONS
    /** Construct a connection string from the attributes.
     * @return    A valid connection string for 'pqxx::conn()'
     */
    std::string     getConnectionString() const
    {
        std::ostringstream oss;
        oss << "host=" << hostname
            << " port=" << port
            << " user=" << username
            << " password=" << password
            << " dbname=" << database;
        return  oss.str();
    }

private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_DATABASECONFIG_H_ */
