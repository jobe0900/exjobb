/*
 * Graph_test.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

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
	    Edge e1(1,1,2);
	    Edge e2(2,2,3);

	    TopoVertexMapType vertexMap;
	    TopoEdgeMapType   edgeMap;

	    vertexMap.emplace(v1.id(), v1);
	    vertexMap.emplace(v2.id(), v2);
	    vertexMap.emplace(v3.id(), v3);
	    edgeMap.emplace(e1.id(), e1);
	    edgeMap.emplace(e2.id(), e2);

	    Topology topo(vertexMap, edgeMap);

		WHEN ("we try create a Graph from the Topology")
		{
			THEN ("we should not get an Exception")
			{
				REQUIRE_NOTHROW (Graph g(topo));
			}
		}
	}
}



