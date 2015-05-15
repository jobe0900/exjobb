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

//			// ...............................................................
//			WHEN ("we try to create postgis topology with a temp name and srid")
//			{
//				PostGisProvider db_handler(config);
//				int srid = 900913;
//				double tolerance = 1.0;
//
//				THEN ("we should not receive an exception")
//				{
//					REQUIRE_NOTHROW (
//						db_handler.buildTopology(srid, tolerance));
//				}
//			}

//			// ...............................................................
//			WHEN ("we try to remove postgis topology with valid arguments")
//			{
//				PostGisProvider db_handler(config);
//
//				THEN ("we should not receive an exception")
//				{
//					REQUIRE_NOTHROW (db_handler.removeTopology());
//				}
//			}
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
