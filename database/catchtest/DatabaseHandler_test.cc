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

		GIVEN ("a valid database configuration structure")
		{
			DatabaseConfig db_config;
			config_reader.getDatabaseConfiguration(db_config);

			// ...............................................................
			WHEN ("we try to connect to the database")
			{
				DatabaseHandler db_handler(db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.getDatabaseVersion());
				}
			}

			// ...............................................................
			WHEN ("we try to install postgis topology extension")
			{
				DatabaseHandler db_handler(db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.installPostgisTopology());
				}
			}

			// ...............................................................
			WHEN ("we try to create postgis topology with a temp name and srid")
			{
				DatabaseHandler db_handler(db_config);
				TimeToStringMaker* p_tts = new EpochMsTimeToString();
				std::string temp_topo = p_tts->getCurrentTimeString();
				delete p_tts;

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.buildTopology(temp_topo, 900913));
				}
			}
		}
	}
	catch (ConfigurationException& e) {
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}

}




