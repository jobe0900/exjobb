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
		const VertexIdType id(1);
		const double x(2);
		const double y(3);
		const Point point(x, y);

		WHEN ("we try to add vertex to Topology")
		{
			const Vertex& r_vertex = topo.addVertex(id, point);

			THEN ("we should get a reference to a TopologyVertex object")
			{
				REQUIRE (r_vertex.id() == id);
				REQUIRE (r_vertex.point() == point);
				REQUIRE (r_vertex.point().x == Approx(x));
				REQUIRE (r_vertex.point().y == Approx(y));
			}
		}
	}

	// -----------------------------------------------------------------------
	GIVEN ("a Topology object and data for 2 vertices with same id")
	{
		Topology topo;
		const VertexIdType v1(1);
		const Point p1(2,3);
		const Point p2(4,5);

		WHEN ("we try to add second vertex to Topology")
		{
			const Vertex& r_v1 = topo.addVertex(v1, p1);
			const Vertex& r_v2 = topo.addVertex(v1, p2);

			THEN ("we should get a reference to first TopologyVertex object")
			{
				REQUIRE (r_v2 == r_v1);
			}
		}
	}

	// -----------------------------------------------------------------------
	GIVEN ("a Topology object and data for two vertices and an edge")
	{
		Topology topo;

		const VertexIdType v1(1);
		const Point p1(2,3);

		const VertexIdType v2(2);
		const Point p2(4,5);

		const EdgeIdType e1(1);

		WHEN ("we try to add edge to Topology with existing vertices")
		{
			const Vertex& r_v1 = topo.addVertex(v1, p1);
			const Vertex& r_v2 = topo.addVertex(v2, p2);

			const Edge& r_edge = topo.addEdge(e1, v1, v2);

			THEN ("we should get a reference to a TopologyEdge object")
			{
				REQUIRE (r_edge.id() == e1);
				REQUIRE (r_edge.source() == r_v1.id());
				REQUIRE (r_edge.target() == r_v2.id());
			}
		}
	}

	// -----------------------------------------------------------------------
	GIVEN ("a Topology object and data for two vertices and an edge")
	{
		Topology topo;

		const VertexIdType v1(1);
		const Point p1(2,3);

		const VertexIdType v2(2);
		const Point p2(4,5);

		const EdgeIdType e1(1);

		WHEN ("we try to add edge to Topology with non-existing vertices")
		{
			THEN ("we should get a TopologyException")
			{
				REQUIRE_THROWS_AS(
						const Edge& r_edge = topo.addEdge(e1, v1, v2),
						TopologyException&
				);
			}
		}
	}
}



