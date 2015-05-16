/*
 * Vertex.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Vertex.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Vertex::Vertex(VertexIdType id, Point point)
	: mId(id), mPoint(point)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Vertex& rVertex)
{
	os  << "TopologyVertex [id: " << rVertex.mId
		<< ", point: " << rVertex.mPoint << "]";
	return os;
}

bool
Vertex::operator==(const Vertex& rhs) const
{
	return (rhs.mId == mId) && (rhs.mPoint == mPoint);
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
VertexIdType
Vertex::id() const
{ return mId; }

Point
Vertex::point() const
{ return mPoint; }

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////





