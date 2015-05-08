/*
 * ConfigurationReader_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"
#include "../ConfigurationReader.h"
#include "../ConfigurationException.h"

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
				REQUIRE (db_config.mUsername == "tester");
				REQUIRE (db_config.mPassword == "tester");
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
						ConfigurationException&);
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
						ConfigurationException&);
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a valid configuration file")
	{
		std::string filename("config/catchtest/testsettings.json");

		WHEN ("asking for vehicle configuration")
		{
			ConfigurationReader config_reader(filename);
			VehicleConfig config;
			config_reader.getVehicleConfiguration(config);

			THEN ("we get a vehicle configuration filled out")
			{
				REQUIRE (config.mCategory == "motorcar");
				REQUIRE (config.mHeight == Approx(1.6));
				REQUIRE (config.mLength == Approx(4.5));
				REQUIRE (config.mWeight == Approx(2.0));
				REQUIRE (config.mWidth == Approx(1.9));
			}
		}
	}
}



