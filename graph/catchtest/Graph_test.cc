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
	GIVEN ("Three points and to edges for a topology")
	{
	    Vertex v1(1, Point(0,0));
	    Vertex v2(2, Point(1,2));
	    Vertex v3(3, Point(3,4));
	    size_t nr_vertices = 3;
	    Edge e1(1,1,2);
	    Edge e2(2,2,3);
	    size_t nr_edges = 2 * 2; // bidirectional

	    Topology topology;
	    topology.addVertex(v1);
	    topology.addVertex(v2);
	    topology.addVertex(v3);
	    topology.addEdge(e1);
	    topology.addEdge(e2);

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
			THEN ("the # of vertices and edges in the graph representation"
			    " should be as in the topology")
			{
			    const auto& boost_graph = g.getRepresentation();
			    REQUIRE (boost::num_vertices(boost_graph) == nr_vertices);
			    REQUIRE (boost::num_edges(boost_graph) == nr_edges);
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
	}
}
