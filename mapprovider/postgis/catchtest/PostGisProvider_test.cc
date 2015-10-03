/*
 * DatabaseHandler_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../../postgis/PostGisProvider.h"
#include "../../postgis/RestrictionQueries.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../../../catchtest/catch.hpp"
#include "../../../config/ConfigurationReader.h"
#include "../../../config/DatabaseConfig.h"
#include "../../../util/TimeToStringMaker.h"
#include "../../../graph/Edge.h"
#include "../../../graph/Vertex.h"
#include "../../../graph/GraphBuilder.h"

SCENARIO ("PostGis topology handling", "[postgis][topology]")
{
	try
	{

		// ===================================================================
		GIVEN ("a configuration file with NO topology name")
		{
		    WHEN ("we try to read in topology")
            {
		        std::string config_file("catchtest/testsettings/missing-topo-testsettings.json");
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

		// ===================================================================
		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
//		    std::string config_file("config/catchtest/testsettings.json");
		    std::string config_file("catchtest/testsettings/mikh0522-testsettings.json");
		    ConfigurationReader config_reader(config_file);
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

			// ...............................................................
			WHEN ("we try to build a graph ")
			{
			    Topology topology;
			    db_handler.getTopology(topology);
			    Configuration config;
			    GraphBuilder graph(topology, config);
			    std::ostringstream oss;

			    THEN ("we should be able to print some information")
			    {
			        graph.printGraphInformation(oss);
			        INFO(oss.str());
			        REQUIRE (true);
			    }
			}

			// ...............................................................
			WHEN ("fetching an edge from topology")
			{
			    Topology topology;
			    db_handler.getTopology(topology);
			    const Edge& edge = topology.getEdge(1);

				THEN ("we should be able to print it out")
				{
				    INFO (edge);
					REQUIRE (true);

					/* Information matches this query:
$ psql -U jonas -d mikh_0522 -c
"SELECT edge_id, osm_id, start_node, end_node, lanes, highway
 FROM   topo_test.edge_data
 JOIN (
 SELECT osm_id, element_id, highway, lanes
 FROM topo_test.relation
 JOIN highways_test
 ON topogeo_id = (topo_geom).id )
 AS osm
 ON edge_id = element_id
 WHERE edge_id = 1;"

 edge_id |  osm_id   | start_node | end_node | lanes |   highway
---------+-----------+------------+----------+-------+-------------
       1 | 124227193 |          1 |       54 |       | residential
(1 row)

					 */
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

SCENARIO ("Set costs on Edges", "[postgis][cost]")
{
	try
	{
		// ===================================================================
		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
		    std::string config_file("catchtest/testsettings/mikh_restr_0617-testsettings.json");
		    ConfigurationReader config_reader(config_file);
		    Configuration config;
		    config_reader.fillConfiguration(config);

			PostGisProvider pgp(config);

			Topology topology;
			pgp.getTopology(topology);

			// ...............................................................
			WHEN ("we try to set restrictions and costs on topology")
			{
			    pgp.setRestrictionsAndCosts(topology);

			    THEN ("we should be able to read travel time cost on edges")
			    {
			        EdgeIdType id = 1;
			        const Edge& edge = topology.getEdge(id);
			        INFO ("edge " << id
			            << ", length: " << edge.geomData().length
			            << ", travel time: "
			            << edge.edgeCost().getCost(EdgeCost::TRAVEL_TIME)
			            << ", total cost: " << edge.cost());
                    INFO ("edge " << edge);
			        REQUIRE (edge.cost() > 0);
			    }

                THEN ("we should be able to find cost for barriers")
                {
                    EdgeIdType id = 869;
                    const Edge& edge = topology.getEdge(id);
                    INFO ("edge " << id
                        << ", length: " << edge.geomData().length
                        << ", travel time: "
                        << edge.edgeCost().getCost(EdgeCost::TRAVEL_TIME)
                        << ", barrier cost: "
                        << edge.edgeCost().getCost(EdgeCost::BARRIER)
                        << ", total cost: " << edge.cost());
                    REQUIRE (edge.cost() > 0);
                }

                THEN ("we should be able to find cost for other hindrances")
                {
                    EdgeIdType id = 869;
                    const Edge& edge = topology.getEdge(id);
                    INFO ("edge " << id
                        << ", length: " << edge.geomData().length
                        << ", travel time: "
                        << edge.edgeCost().getCost(EdgeCost::TRAVEL_TIME)
                        << ", barrier cost: "
                        << edge.edgeCost().getCost(EdgeCost::BARRIER)
                        << ", other cost: "
                        << edge.edgeCost().getCost(EdgeCost::OTHER)
                        << ", total cost: " << edge.cost());
                    REQUIRE (edge.cost() > 0);
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

