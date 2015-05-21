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

	    Topology topology;
	    const Vertex& v1 = topology.addVertex(1, Point(0,0));
	    const Vertex& v2 = topology.addVertex(2, Point(1,2));
	    const Vertex& v3 = topology.addVertex(3, Point(3,1));
	    const Edge& e1 = topology.addEdge(1,1,2);
	    const Edge& e2 = topology.addEdge(2,2,3);

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
			THEN ("the # of vertices in the graph representation"
			    " should be as in the topology"
			    " and the # edges the double") // default is bidirectional
			{
			    const auto& boost_graph = g.getBGLGraph();
			    REQUIRE (boost::num_vertices(boost_graph) == nr_vertices);
			    REQUIRE (boost::num_edges(boost_graph) == nr_edges * 2);
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
		    size_t nr_vertices = 3;
		    size_t nr_edges = 2 * 2; // with bidirectional information

		    Topology topo2;
		    const Vertex& v1 = topo2.addVertex(1, Point(0,0));
		    const Vertex& v2 = topo2.addVertex(2, Point(1,2));
		    const Vertex& v3 = topo2.addVertex(3, Point(3,1));

		    Edge::RoadData rd1;
		    rd1.direction = Edge::DirectionType::FROM_TO;
		    Edge& e1 = topo2.addEdge(1,1,2);
		    e1.setRoadData(rd1);

		    Edge::RoadData rd2;
		    rd2.direction = Edge::DirectionType::TO_FROM;
		    Edge& e2 = topo2.addEdge(2,2,3);
		    e2.setRoadData(rd2);

		    INFO (e1);
		    INFO (e2);
		    REQUIRE (topo2.nrVertices() == 3);
		    REQUIRE (topo2.nrEdges() == 2);

		    Graph g2(topo2);

		    THEN ("the # of edges in the graph representation"
		          " should as many as in the topology")
		    {
		        INFO (g2)
		        const auto& boost_graph = g2.getBGLGraph();
		        REQUIRE (boost::num_edges(boost_graph) == topo2.nrEdges());
		    }
		}
	}
}
