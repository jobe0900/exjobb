/*
 * MapProvider_test.cc
 *
 *  Created on: 2015-05-13
 *      Author: Jonas Bergman
 */

#include "../MapProvider.h"

#include <string>
#include <map>

#include "../../catchtest/catch.hpp"
#include "../../config/ConfigurationReader.h"
#include "../../config/DatabaseConfig.h"
#include "../postgis/PostGisProvider.h"
#include "../../graph/TopologyEdge.h"
#include "../../graph/TopologyVertex.h"


SCENARIO ("MapProvider queries", "[mp-query]")
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

			MapProvider* p_mp = new PostGisProvider(topo_name, db_config);

			// ...............................................................
			WHEN ("we try to fetch topology vertices")
			{
				std::map<VertexId, TopologyVertex> vertex_map;
				p_mp->getTopologyVertices(vertex_map);
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
				p_mp->getTopologyEdges(edge_map);
				size_t nr_edges = edge_map.size();

				THEN ("we should receive a list of TopologyEdge")
				{
					REQUIRE (nr_edges > 0);
				}
			}
			delete p_mp;
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
