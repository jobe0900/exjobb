/*
 * TopologyVertex.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "TopologyVertex.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
TopologyVertex::TopologyVertex(VertexId id, Point point)
	: mId(id), mPoint(point)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const TopologyVertex& rVertex)
{
	os  << "TopologyVertex [id: " << rVertex.mId
		<< ", point: " << rVertex.mPoint << "]";
	return os;
}

bool
TopologyVertex::operator==(const TopologyVertex& rhs) const
{
	return (rhs.mId == mId) && (rhs.mPoint == mPoint);
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
VertexId
TopologyVertex::id() const
{ return mId; }

Point
TopologyVertex::point() const
{ return mPoint; }

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////





