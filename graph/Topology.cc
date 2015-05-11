/*
 * Topology.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Topology.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Topology::Topology()
{
}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
const TopologyVertex&
Topology::addVertex(TopologyId id, Point point)
{
	auto res = mVertexMap.emplace(id, TopologyVertex(id, point));
	return res.first->second;
}


const TopologyEdge&
Topology::addEdge(EdgeId id, VertexId source, VertexId target)
{
	try
	{
		const TopologyVertex& r_source = getVertex(source);
		const TopologyVertex& r_target = getVertex(target);
	}
	catch (TopologyException& e)
	{
		throw TopologyException("Cannot add edge: " + std::to_string(id) +
				". " + e.what());
	}
	auto res = mEdgeMap.emplace(id, TopologyEdge(id, r_source, r_target));
	return res.first->second;
}


const TopologyVertex&
Topology::getVertex(VertexId id) const
{
	auto it = mVertexMap.find(id);
	if(it == mVertexMap.end()) {
		throw TopologyException("Vertex not found: " + std::to_string(id));
	}
	return *it;
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




