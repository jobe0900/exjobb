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
	 *
	 * @param	rFilename	The filename for the configuration json file
	 * @throw	boost::property_tree::json_parser_error		If invalid file
	 */
	ConfigurationReader(const std::string& rFilename);



	// OPERATORS
	// OPERATIONS

	/** Read the database part of the configuration and populate config struct.
	 *
	 * @param	rDatabaseConfig		The configuration structure to populate.
	 * @throw	boost::property_tree::ptree_error	If missing configuration.
	 */
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
