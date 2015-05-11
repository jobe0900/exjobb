/*
 * DatabaseHandler_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include <string>
#include <vector>

#include "../../catchtest/catch.hpp"
#include "../../config/ConfigurationReader.h"
#include "../../config/DatabaseConfig.h"
#include "../DatabaseHandler.h"
#include "../../util/EpochMsTimeToString.h"
#include "../../graph/TopologyEdge.h"
#include "../../graph/TopologyVertex.h"

SCENARIO ("PostGis topology handling", "[.topology]") // DISABLED TEST BY .
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
			WHEN ("we try to remove postgis topology with valid arguments")
			{
				DatabaseHandler db_handler(db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.removeTopology(temp_topo));
				}
			}

			// ...............................................................
			WHEN ("we try to remove postgis topology with invalid arguments")
			{
				DatabaseHandler db_handler(db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.removeTopology("foo"));
				}
			}
		}
	}
	catch (ConfigurationException& e) {
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}
}


SCENARIO ("PostGis queries", "[query]")
{
	try
	{
		std::string config_file("config/catchtest/testsettings.json");
		ConfigurationReader config_reader(config_file);

		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
			DatabaseConfig db_config;
			config_reader.getDatabaseConfiguration(db_config);
			std::string topo_name("test");

			DatabaseHandler db_handler(db_config);

			try
			{
				db_handler.buildTopology(topo_name, 900913, 1.0);
			}
			catch(DatabaseException& e)
			{
//				INFO ("Topology" << topo_name << " exists.");
			}

			// ...............................................................
			WHEN ("we try to fetch topology vertices")
			{
				Topology topo;
				db_handler.getTopologyVertices(topo_name, topo);
				size_t nr_vertices = topo.nrVertices();

//				if(nr_vertices > 0) {
//					INFO ("First vertex " << *topo_vertices.at(0));
//					REQUIRE (true); // force output
//				}

				THEN ("we should receive a list of TopologyVertex")
				{
					REQUIRE (nr_vertices > 0);
				}
			}

			// ...............................................................
			WHEN ("we try to fetch topology edges")
			{
				Topology topo;
				db_handler.getTopologyEdges(topo_name, topo);
				size_t nr_edges = topo.nrEdges();

//				if(nr_edges > 0) {
//					INFO ("First edge " << *topo_edges.at(0));
//					REQUIRE (true); // force output
//				}

				THEN ("we should receive a list of TopologyEdge")
				{
					REQUIRE (nr_edges > 0);
				}
			}
		}
	}
	catch (ConfigurationException& e)
	{
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}
	catch (DatabaseException& dbe)
	{
		INFO(dbe.what());
		REQUIRE (false);	// force output of error and failure

	}
}
