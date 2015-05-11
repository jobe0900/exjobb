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
	GIVEN ("a Topology object and data for a vertex")
	{
		Topology topo;
		TopologyId id(1);
		double x = 2, y = 3;
		Point point(x, y);

		WHEN ("we try to add vertex to Topology")
		{
			const TopologyVertex& r_vertex = topo.addVertex(id, point);

			THEN ("we should get a reference to a TopologyVertex object")
			{
				REQUIRE (r_vertex.mId == id);
				REQUIRE (r_vertex.mPoint.x == Approx(x));
				REQUIRE (r_vertex.mPoint.y == Approx(y));
			}
		}
	}
}



