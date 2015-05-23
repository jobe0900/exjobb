/*
 * Graph_test.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include <iostream>

#include "../../catchtest/catch.hpp"

#include "../Graph.h"
#include "../Topology.h"

SCENARIO ("Building a small graph", "[graph]")
{
	// -----------------------------------------------------------------------
	GIVEN ("Three points and two edges for a topology")
	{
	    size_t nr_vertices = 3;
	    size_t nr_edges = 2;
	    OsmIdType osm_id(0);

	    Topology topology;
	    const Vertex& v1 = topology.addVertex(1, Point(0,0));
	    const Vertex& v2 = topology.addVertex(2, Point(1,2));
	    const Vertex& v3 = topology.addVertex(3, Point(3,1));
	    Edge& e1 = topology.addEdge(1,osm_id,1,2);
	    Edge& e2 = topology.addEdge(2,osm_id,2,3);

	    // ...................................................................
		WHEN ("we try create a Graph from the Topology")
		{
			THEN ("we should not get an Exception")
			{
				REQUIRE_NOTHROW (Graph g(topology));
			}
		}

	    // ...................................................................
		WHEN ("building a graph from the topology")
		{
		    Graph g(topology);
		    const auto& boost_graph = g.getBoostGraph();
		    const auto& boost_line_graph = g.getBoostLineGraph();

		    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
			THEN ("the # of vertices in the graph representation"
			    " should be as in the topology"
			    " and the # edges the double") // default is bidirectional
			{
			    REQUIRE (boost::num_vertices(boost_graph) == nr_vertices);
			    REQUIRE (boost::num_edges(boost_graph) == nr_edges * 2);
			}

		    // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
			THEN ("the number of nodes in the LineGraph"
			      " should be as many as edges in the graph")
			{
			    REQUIRE (boost::num_vertices(boost_line_graph) ==
			             boost::num_edges(boost_graph));
			}
		}

		// ...................................................................
		WHEN ("we try print out a Graph from the Topology")
		{
		    Graph g(topology);

		    THEN ("we should get a print out")
		    {
		        INFO (g);
		        REQUIRE (true);
		    }
		}

		// ...................................................................
		WHEN ("adding unidirectional information to edges before"
		      " building graph")
		{

		    Edge::RoadData rd1;
		    rd1.direction = Edge::DirectionType::FROM_TO;
		    e1.setRoadData(rd1);

		    Edge::RoadData rd2;
		    rd2.direction = Edge::DirectionType::FROM_TO;
		    e2.setRoadData(rd2);

		    Graph g2(topology);

		    THEN ("the # of edges in the graph representation"
		          " should as many as in the topology")
		    {
		        INFO (g2)
		        const auto& boost_graph = g2.getBoostGraph();
		        REQUIRE (boost::num_edges(boost_graph) == topology.nrEdges());
		    }
		}

		// ...................................................................
		WHEN ("adding an extra lane to an edge before"
		      " building graph")
		{

		    Edge::RoadData rd1;
		    rd1.direction = Edge::DirectionType::FROM_TO;
		    rd1.nrLanes = 2;
		    e1.setRoadData(rd1);

		    Edge::RoadData rd2;
		    rd2.direction = Edge::DirectionType::FROM_TO;
		    e2.setRoadData(rd2);

		    Graph g2(topology);

		    THEN ("the # of edges in the graph representation"
		        " should be one more than in the topology")
		    {
		        INFO (g2)
		                    const auto& boost_graph = g2.getBoostGraph();
		        REQUIRE (boost::num_edges(boost_graph) == topology.nrEdges() + 1);
		    }
		}
	}
}
