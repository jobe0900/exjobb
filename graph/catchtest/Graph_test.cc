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
#include "../TopoEdgeData.h"

SCENARIO ("Building a small graph", "[graph]")
{
	// -----------------------------------------------------------------------
	GIVEN ("Three points and to edges for a topology")
	{
	    size_t nr_vertices = 3;
	    size_t nr_edges = 2; // no TopoEdgeData with directional information

	    Topology topology;
	    const Vertex& v1 = topology.addVertex(1, Point(0,0));
	    const Vertex& v2 = topology.addVertex(2, Point(1,2));
	    const Vertex& v3 = topology.addVertex(3, Point(3,4));
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

		WHEN ("adding bidirectional information to edges before"
		      " building topology and graph")
		{
		    TopoEdgeData* p_ed1 = new TopoEdgeData();
		    p_ed1->setDirection(TopoEdgeData::Direction::BOTH);

		    TopoEdgeData* p_ed2 = new TopoEdgeData();
		    p_ed2->setDirection(TopoEdgeData::Direction::BOTH);

//		    Vertex v1(1, Point(0,0));
//		    Vertex v2(2, Point(1,2));
//		    Vertex v3(3, Point(3,4));
		    size_t nr_vertices = 3;
//		    Edge e1(1,1,2);
//		    Edge e2(2,2,3);
		    size_t nr_edges = 2 * 2; // with bidirectional information

		    Topology topo2;
		    const Vertex& v1 = topo2.addVertex(1, Point(0,0));
		    const Vertex& v2 = topo2.addVertex(2, Point(1,2));
		    const Vertex& v3 = topo2.addVertex(3, Point(3,4));
		    Edge& e1 = topo2.addEdge(1,1,2);
		    Edge& e2 = topo2.addEdge(2,2,3);
		    e1.setEdgeData(p_ed1);
		    e2.setEdgeData(p_ed2);

		    INFO (e1);
		    INFO (e2);
		    REQUIRE (topo2.nrVertices() == 3);
		    REQUIRE (topo2.nrEdges() == 2);

		    Graph g2(topo2);

		    THEN ("the # of edges in the graph representation"
		          " should be twice as many as in the topology")
		    {
		        INFO (g2)
		        const auto& boost_graph = g2.getRepresentation();
		        REQUIRE (boost::num_edges(boost_graph) == topo2.nrEdges() * 2);
		    }
		}
	}
}
