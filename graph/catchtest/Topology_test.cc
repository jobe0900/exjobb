/*
 * Topology_test.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"

#include "../Topology.h"

SCENARIO ("Storing topology edges and vertices in Topology", "[topology]")
{
	// -----------------------------------------------------------------------
	GIVEN ("a Topology object and data for a vertex")
	{
		Topology topo;
		const TopologyId id(1);
		const double x = 2;
		const double y = 3;
		const Point point(x, y);

		WHEN ("we try to add vertex to Topology")
		{
			const TopologyVertex& r_vertex = topo.addVertex(id, point);

			THEN ("we should get a reference to a TopologyVertex object")
			{
				REQUIRE (r_vertex.mId == id);
				REQUIRE (r_vertex.mPoint == point);
				REQUIRE (r_vertex.mPoint.x == Approx(x));
				REQUIRE (r_vertex.mPoint.y == Approx(y));
			}
		}
	}

	// -----------------------------------------------------------------------
	GIVEN ("a Topology object and data for two vertices and an edge")
	{
		Topology topo;
		const VertexId v1(1);
		const double x = 2;
		const double y = 3;
		const Point p1(x, y);

		const VertexId v2(2);
		const Point p2(4,5);

		const EdgeId e1(1);
		const VertexId source(1);
		const VertexId target(2);

		WHEN ("we try to add edge to Topology with existing vertices")
		{
			const TopologyVertex& r_v1 = topo.addVertex(v1, p1);
			const TopologyVertex& r_v2 =topo.addVertex(v2, p2);

			const TopologyEdge& r_edge = topo.addEdge(e1, v1, v2);

			THEN ("we should get a reference to a TopologyEdge object")
			{
				REQUIRE (r_edge.id() == e1);
				REQUIRE (r_edge.source() == r_v1);
				REQUIRE (r_edge.target() == r_v2);
			}
		}
	}
}



