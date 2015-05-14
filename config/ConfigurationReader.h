/**  Read configurations from a json file.
 *
 * #include "ConfigurationReader.h"
 *
 * @author Jonas Bergman
 */
#ifndef CONFIG_CONFIGURATIONREADER_H_
#define CONFIG_CONFIGURATIONREADER_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// LOCAL INCLUDES
//
#include "Configuration.h"
#include "DatabaseConfig.h"
#include "ConfigurationException.h"
#include "VehicleConfig.h"

// FORWARD REFERENCES
//

/**
 * A class to handle the reading of data from a json configuration file.
 */
class ConfigurationReader
{
public:
// LIFECYCLE
	/** Default constructor.
	 */
	ConfigurationReader() = delete;

	/** Constructor.
	 * Always initialize a Configuration reader with the configuration file.
	 *
	 * @param	rFilename	The filename for the configuration json file
	 * @throw	ConfigurationException		If invalid file
	 */
	ConfigurationReader(const std::string& rFilename);



// OPERATORS
// OPERATIONS

	/** Get the configurations from the file.
	 * @return Pointer to a Configuration.
	 * @throws ConfigurationException
	 */
	const Configuration* getConfiguration() const;

	/** Read the database part of the configuration and populate config struct.
	 *
	 * @param	rDatabaseConfig		The configuration structure to populate.
	 * @throw	ConfigurationException	If missing configuration.
	 */
//	void	getDatabaseConfiguration(DatabaseConfig& rDatabaseConfig);

	/** Read the vehicle part of the configuration and populate config struct.
	 *
	 * @param	rVehicleConfig		The configuration structure to populate.
	 * @throw	ConfigurationException	If missing configuration.
	 */
//	void	getVehicleConfiguration(VehicleConfig& rVehicleConfig);

// ACCESS
// INQUIRY

protected:

private:
// ATTRIBUTES
	std::string					mFilename;
	boost::property_tree::ptree	mPropertyTree;

// HELPERS
	/** Read the database part of the configuration and populate config struct.
	 * @return	The Database configuration
	 * @throw	ConfigurationException	If missing configuration.
	 */
	const DatabaseConfig*   getDatabaseConfiguration() const;
	/** Read the vehicle part of the configuration and populate config struct.
	 * @return  The Vehicle configuration
	 * @throw   ConfigurationException  If missing configuration.
	 */
	const VehicleConfig*    getVehicleConfiguration() const;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_CONFIGURATIONREADER_H_ */
