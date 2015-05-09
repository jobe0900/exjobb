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
#include "../DatabaseHandler.h"
#include "../../util/EpochMsTimeToString.h"

SCENARIO ("Communicate with database", "[database]")
{
	try
	{
		std::string config_file("config/catchtest/testsettings.json");
		ConfigurationReader config_reader(config_file);

		GIVEN ("a valid database configuration structure and a temporary name")
		{
			DatabaseConfig db_config;
			config_reader.getDatabaseConfiguration(db_config);
			TimeToStringMaker* p_tts = new EpochMsTimeToString();
			std::string temp_topo = p_tts->getCurrentTimeString();
			delete p_tts;

			// ...............................................................
			WHEN ("we try to create postgis topology with a temp name and srid")
			{
				DatabaseHandler db_handler(db_config);
				int srid = 900913;
				double tolerance = 1.0;

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (
						db_handler.buildTopology(temp_topo, srid, tolerance));
				}
			}

			// ...............................................................
			WHEN ("we try to remove postgis topology valid arguments")
			{
				DatabaseHandler db_handler(db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (
						db_handler.removeTopology(temp_topo));
				}
			}
		}
	}
	catch (ConfigurationException& e) {
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}

}
