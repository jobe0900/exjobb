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

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




