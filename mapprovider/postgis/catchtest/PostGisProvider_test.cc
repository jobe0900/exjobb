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
#include "../../../graph/TopologyEdge.h"
#include "../../../graph/TopologyVertex.h"

SCENARIO ("PostGis topology handling", "[topology]")
{
	try
	{
		std::string config_file("config/catchtest/testsettings.json");
		ConfigurationReader config_reader(config_file);

		GIVEN ("a valid database configuration structure and a temporary name")
		{
		    Configuration config;
		    config_reader.fillConfiguration(config);
			const DatabaseConfig& db_config = config.getDatabaseConfig();
			std::string temp_topo(TimeToStringMaker::getEpochMsTimeString());

			// ...............................................................
			WHEN ("we try to create postgis topology with a temp name and srid")
			{
				PostGisProvider db_handler(temp_topo, db_config);
				int srid = 900913;
				double tolerance = 1.0;

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (
						db_handler.buildTopology(srid, tolerance));
				}
			}

			// ...............................................................
			WHEN ("we try to remove postgis topology with valid arguments")
			{
				PostGisProvider db_handler(temp_topo, db_config);

				THEN ("we should not receive an exception")
				{
					REQUIRE_NOTHROW (db_handler.removeTopology());
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
		    Configuration config;
		    config_reader.fillConfiguration(config);
			const DatabaseConfig& db_config = config.getDatabaseConfig();
			std::string topo_name("test");

			PostGisProvider db_handler(topo_name, db_config);

			try
			{
				db_handler.buildTopology(900913, 1.0);
			}
			catch(MapProviderException& e)
			{
//				INFO ("Topology" << topo_name << " exists.");
			}

			// ...............................................................
			WHEN ("we try to fetch topology vertices")
			{
				std::map<VertexId, TopologyVertex> vertex_map;
				db_handler.getTopologyVertices(vertex_map);
				size_t nr_vertices = vertex_map.size();

				THEN ("we should receive a list of TopologyVertex")
				{
					REQUIRE (nr_vertices > 0);
				}
			}

			// ...............................................................
			WHEN ("we try to fetch topology edges")
			{
				std::map<EdgeId, TopologyEdge> edge_map;
				db_handler.getTopologyEdges(edge_map);
				size_t nr_edges = edge_map.size();

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
	catch (MapProviderException& dbe)
	{
		INFO(dbe.what());
		REQUIRE (false);	// force output of error and failure

	}
}
