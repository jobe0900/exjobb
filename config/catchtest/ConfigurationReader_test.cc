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
				REQUIRE (db_config.hostname == "127.0.0.1");
				REQUIRE (db_config.port == 5432);
				REQUIRE (db_config.username == "tester");
				REQUIRE (db_config.password == "tester");
				REQUIRE (db_config.database == "mikh_style");
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
				REQUIRE (config.category == "motorcar");
				REQUIRE (config.height == Approx(1.6));
				REQUIRE (config.length == Approx(4.5));
				REQUIRE (config.weight == Approx(2.0));
				REQUIRE (config.width == Approx(1.9));
			}
		}
	}
}



