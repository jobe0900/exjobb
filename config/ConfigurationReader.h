/**  Read configurations from a json file.
 *
 * #include "ConfigurationReader.h" <BR>
 * -llib
 *
 * A longer description.
 *
 * @see something
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
#include "DatabaseConfig.h"

// FORWARD REFERENCES
//


class ConfigurationReader
{
public:
	// LIFECYCLE

	/** Constructor.
	 * Always initialize a Configuration reader with the configuration file.
	 */
	ConfigurationReader(std::string& filename);



	// OPERATORS
	// OPERATIONS

	void	getDatabaseConfiguration(DatabaseConfig& rDatabaseConfig);

	// ACCESS
	// INQUIRY

protected:

private:
	/** Default constructor.
	 * Hidden.
	 */
	ConfigurationReader();

	// ATTRIBUTES
	std::string					mFilename;
	boost::property_tree::ptree	mPropertyTree;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_CONFIGURATIONREADER_H_ */
