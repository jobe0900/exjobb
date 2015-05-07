/*
 * DatabaseHandler_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include <string>

#include "../../catchtest/catch.hpp"
#include "../../config/ConfigurationReader.h"
#include "../../config/DatabaseConfig.h"

SCENARIO ("Communicate with database", "[database]")
{
	try
	{
		std::string config_file("../../config/catchtest/testsettings.json");
		ConfigurationReader config_reader(config_file);

		GIVEN ("a valid database configuration structure")
		{
			DatabaseConfig db_config;
			config_reader.getDatabaseConfiguration(db_config);

			WHEN ("we try to connect to the database")
			{
				DatabaseHandler db_handler(db_config);
				db_handler.getDbVersion();
				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.getDbVersion());
				}
			}
		}
	}
	catch (ConfigurationException& e) {
		INFO(e.what());
		REQUIRE (true);	// force output
	}

}




