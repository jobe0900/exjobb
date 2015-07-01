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
#include "../../../graph/Graph.h"

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

		// ===================================================================
		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
//		    std::string config_file("config/catchtest/testsettings.json");
		    std::string config_file("mapprovider/postgis"
		        "/catchtest/mikh0522-testsettings.json");
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
			    Graph graph(topology);
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

SCENARIO ("Fetch restrictions from PostGis ", "[postgis][restrictions]")
{
	try
	{

		// ===================================================================
		GIVEN ("a valid database configuration to a PostGisProvider ")
		{
//		    std::string config_file("mapprovider/postgis"
//		        "/catchtest/mikh0530-testsettings.json");
		    std::string config_file("catchtest/mikh_restr_0617-testsettings.json");
		    ConfigurationReader config_reader(config_file);
		    Configuration config;
		    config_reader.fillConfiguration(config);

			PostGisProvider pgp(config);
			Topology topology;
			pgp.getTopology(topology);

			// ...............................................................
			WHEN ("we try to fetch Restrictions")
			{
			    Restrictions restrictions;

			    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should not get an exception")
				{
					REQUIRE_NOTHROW (
					    pgp.getRestrictions(restrictions, topology));
				}
			}

			WHEN ("trying to fetch restrictions")
			{
			    Restrictions restrictions;
			    pgp.getRestrictions(restrictions, topology);
			    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a map of vehicle property restrictions")
				{

				    const auto& vpr_map =
				        restrictions.edgeRestrictions().vehicleProperties();
				    if(vpr_map.size() > 0)
				    {
				        const EdgeRestrictions::VehicleProperties& vp =
				            vpr_map.begin()->second;
				        INFO ("# VehiclePropertyRestricitons: " << vpr_map.size());
				        INFO ("  First restriction on edge id: "
				              << vpr_map.begin()->first
				              << "\n   with maxspeed: " << vp.maxSpeed
				              << ", minspeed: " << vp.minSpeed
				              << ", maxheight: " << vp.maxHeight
				              << ", maxlength: " << vp.maxLength
				              << ", maxweight: " << vp.maxWeight
				              << ", maxwidth: " << vp.maxWidth
				              );
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No VehiclePropertyRestrictions.");
				        REQUIRE (true);
				    }
				}

			    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a map of general access restrictions")
				{
				    const auto& general_map =
				        restrictions.edgeRestrictions().generalAccess();
				    if(general_map.size() > 0)
				    {
				        EdgeIdType edgeId = general_map.begin()->first;
				        const OsmAccess& access = general_map.begin()->second;
				        INFO ("# General access restrictions: "
				              << general_map.size());
				        INFO ("  First restriction on edge id: " << edgeId
				              << ", access restriction: " << access.toString());
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No General Access Restrictions.");
				        REQUIRE (true);
				    }
				}

				// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a map of vehicle restrictions")
				{
				    const auto& edge_map =
				        restrictions.edgeRestrictions().vehicleTypeAccessEdges();
				    if(edge_map.size() > 0)
				    {
				        EdgeIdType edgeId = edge_map.begin()->first;
				        const auto& v_map = edge_map.begin()->second;
				        INFO ("# edges with vehicle restrictions: "
				            << edge_map.size());
				        INFO ("# vehicle restrictions on first edge: "
				            << v_map.size());
				        INFO ("  First restriction on edge id: " << edgeId
				            << ", vehicle restriction: "
				            << OsmVehicle::toString(v_map.begin()->first)
				            << " : "
				            << v_map.begin()->second.toString());
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No Vehicle Restrictions.");
				        REQUIRE (true);
				    }
				}

				// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a map of barrier restrictions")
				{
				    const auto& barrier_map =
				        restrictions.edgeRestrictions().barriers();
				    if(barrier_map.size() > 0)
				    {
				        EdgeIdType edgeId = barrier_map.begin()->first;
				        const OsmBarrier& barrier = barrier_map.begin()->second;
				        INFO ("# Barrier restrictions: "
				            << barrier_map.size());
				        INFO ("  First restriction on edge id: " << edgeId
				            << ", barrier restriction: " << barrier.toString());
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No Barrier Restrictions.");
				        REQUIRE (true);
				    }
				}

				// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a set of edgeIds of disused edges")
				{
				    const auto& disused_set =
				        restrictions.edgeRestrictions().disusedEdges();
				    if(disused_set.size() > 0)
				    {
				        EdgeIdType edgeId = *disused_set.begin();
				        INFO ("# Disused edges: " << disused_set.size());
				        INFO ("  First disused edge id: " << edgeId);
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No Disused edges.");
				        REQUIRE (true);
				    }
				}

				// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
				THEN ("we should get a set of edgeIds of 'noexit' edges")
				{
				    const auto& noexit_set =
				        restrictions.edgeRestrictions().noExitEdges();
				    if(noexit_set.size() > 0)
				    {
				        EdgeIdType edgeId = *noexit_set.begin();
				        INFO ("# 'Noexit' edges: " << noexit_set.size());
				        INFO ("  First 'noexit' edge id: " << edgeId);
				        REQUIRE (true);
				    }
				    else
				    {
				        INFO ("No 'noexit' edges.");
				        REQUIRE (true);
				    }
				}
			}

			//................................................................
			WHEN ("parsing query result of edge ids")
			{
			    std::string edgesString("{123,345,567,789}");

			    THEN ("we shoud be able to split the string")
			    {
			        std::vector<EdgeIdType> edgeIds =
			            RestrictionQueries::Results::parseEdgeIdsString(
			                edgesString);
			        REQUIRE (edgeIds.size() == 4);
			        REQUIRE (edgeIds[0] == 123);
			        REQUIRE (edgeIds[1] == 345);
			        REQUIRE (edgeIds[2] == 567);
			        REQUIRE (edgeIds[3] == 789);
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
