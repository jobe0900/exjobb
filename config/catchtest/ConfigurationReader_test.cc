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
}



