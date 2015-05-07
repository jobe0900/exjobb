/*
 * ConfigurationReader_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"
#include "../ConfigurationReader.h"

SCENARIO ("Use ConfigurationReader to read configuration from json file",
		"[json],[config]")
{
	//------------------------------------------------------------------------
	GIVEN ("a filename to a valid configuration file")
	{
		std::string filename("config/catchtest/testsettings.json");

		WHEN ("asking for database configuration")
		{
			ConfigurationReader config_reader(filename);
			DatabaseConfig db_config;
			config_reader.getDatabaseConfiguration(db_config);

			THEN ("we get a  database configuration filled out")
			{
				REQUIRE (db_config.mHostname == "127.0.0.1");
				REQUIRE (db_config.mPort == 5432);
				REQUIRE (db_config.mUsername == "jonas");
				REQUIRE (db_config.mPassword == "jonas");
				REQUIRE (db_config.mDatabase == "mikh_style");
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a configuration file with missing information")
	{
		std::string filename("config/catchtest/testsettings-missing-name.json");

		WHEN ("asking for database configuration")
		{
			ConfigurationReader config_reader(filename);
			DatabaseConfig db_config;

			THEN ("we get an exception")
			{
				REQUIRE_THROWS_AS (config_reader.getDatabaseConfiguration(db_config),
						boost::property_tree::ptree_error&);
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a non-existing file")
	{
		std::string filename("config/catchtest/foo.json");

		WHEN ("asking for database configuration")
		{
			THEN ("we get an exception")
			{
				REQUIRE_THROWS_AS (ConfigurationReader config_reader(filename),
						boost::property_tree::json_parser_error&);
			}
		}
	}
}



