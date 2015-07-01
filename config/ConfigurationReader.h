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
#include "ConfigurationException.h"
#include "DatabaseConfig.h"
#include "TopologyConfig.h"
#include "VehicleConfig.h"
#include "../osm/OsmVehicle.h"

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
	 * @param   Reference to a Configuration to populate.
	 * @throws  ConfigurationException
	 */
	void    fillConfiguration(Configuration& rConfig) const;

// ACCESS
// INQUIRY

protected:

private:
// ATTRIBUTES
	std::string					mFilename;
	boost::property_tree::ptree	mPropertyTree;

// HELPERS
	/** Read the database part of the configuration and populate config struct.
	 * @param	The Database configuration
	 * @throw	ConfigurationException	If missing configuration.
	 */
	void    fillDatabaseConfiguration(DatabaseConfig& rDatabaseConfig) const;

	/** Read the topology part of the configuration and populate config struct.
	 * @param	The Topology configuration
	 * @throw	ConfigurationException	If missing configuration.
	 */
	void    fillTopologyConfiguration(TopologyConfig& rTopologyConfig) const;

	/** Read the vehicle part of the configuration and populate config struct.
	 * @param   The Vehicle configuration
	 * @throw   ConfigurationException  If missing configuration.
	 */
	void    fillVehicleConfiguration(VehicleConfig& rVehicleConfig) const;

	/** Read the Cost part of the configuration and populate config struct.
	 * @param   The Cost configuration
	 * @throw   ConfigurationException  If missing configuration.
	 */
	void    fillCostConfiguration(CostConfig& rCostConfig) const;

	/** Helper to `fillCostConfig()`. Fill in the Default Speed part.
	 * @param   The Cost configuration.
	 */
	void    fillDefaultSpeedCost(CostConfig& rCostConfig) const;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_CONFIGURATIONREADER_H_ */
