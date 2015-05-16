/*
 * DatabaseHandler_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../../postgis/PostGisProvider.h"

#include <string>
#include <vector>

#include "../../../catchtest/catch.hpp"
#include "../../../config/ConfigurationReader.h"
#include "../../../config/DatabaseConfig.h"
#include "../../../util/TimeToStringMaker.h"
#include "../../../graph/Edge.h"
#include "../../../graph/Vertex.h"

SCENARIO ("PostGis topology handling", "[postgis][topology]")
{
	try
	{
		// ===================================================================
		GIVEN ("a valid configuration structure with a temporary name")
		{
		    std::string config_file("mapprovider/postgis"
		        "/catchtest/temptable-testsettings.json");
		    ConfigurationReader config_reader(config_file);
		    Configuration config;
		    config_reader.fillConfiguration(config);
		    PostGisProvider* p_pgp(nullptr);

			// ...............................................................
			WHEN ("we try to create postgis topology")
			{
				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW ( p_pgp = new PostGisProvider(config));
				}
			}

			// ...............................................................
			WHEN ("we try to remove postgis topology with valid arguments")
			{
				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (delete p_pgp);
				}
			}
			delete p_pgp;
		}

		// ===================================================================
		GIVEN ("a configuration file with NO topology name")
		{
		    WHEN ("we try to read in topology")
            {
		        std::string config_file("mapprovider/postgis"
		            "/catchtest/missing-topo-testsettings.json");
		        ConfigurationReader config_reader(config_file);
		        Configuration config;
		        config_reader.fillConfiguration(config);

		        THEN ("we should get an exception")
		        {
		            REQUIRE_THROWS_AS (PostGisProvider pgp(config),
		                MapProviderException&);
		        }
            }
		}
	}
	catch (ConfigurationException& e) {
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}
}


SCENARIO ("PostGis queries", "[postgis][query]")
{
	try
	{
		std::string config_file("config/catchtest/testsettings.json");
		ConfigurationReader config_reader(config_file);

		// ===================================================================
		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
		    Configuration config;
		    config_reader.fillConfiguration(config);

			PostGisProvider db_handler(config);

			// ...............................................................
			WHEN ("we try to fetch a topology")
			{
			    Topology topology;

				THEN ("we should not get an exception")
				{
					REQUIRE_NOTHROW (db_handler.getTopology(topology););
				}
			}

			// ...............................................................
			WHEN ("we try to fetch topology ")
			{
			    Topology topology;
			    db_handler.getTopology(topology);

				THEN ("we should receive a vertices and edges")
				{
					REQUIRE (topology.nrVertices() > 0);
					REQUIRE (topology.nrEdges() > 0);
				}
			}
		}
	}
	catch (ConfigurationException& e)
	{
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}
	catch (MapProviderException& dbe)
	{
		INFO(dbe.what());
		REQUIRE (false);	// force output of error and failure

	}
}
