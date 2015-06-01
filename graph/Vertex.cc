/*
 * Vertex.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Vertex.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////
const VertexIdType Vertex::MAX_ID = std::numeric_limits<VertexIdType>::max();

//============================= LIFECYCLE ====================================
Vertex::Vertex(VertexIdType id, Point point, bool hasRestrictions)
	: mId(id), mPoint(point), mHasRestrictions(hasRestrictions)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Vertex& rVertex)
{
	os  << "Vertex [id: " << rVertex.mId
		<< ", point: " << rVertex.mPoint << "]";
	return os;
}

bool
Vertex::operator==(const Vertex& rhs) const
{
	return (rhs.mId == mId) && (rhs.mPoint == mPoint);
}

//============================= OPERATIONS ===================================
void
Vertex::setHasRestrictions(bool restrictions)
{
    mHasRestrictions = restrictions;
}

//============================= ACESS      ===================================
VertexIdType
Vertex::id() const
{ return mId; }

Point
Vertex::point() const
{ return mPoint; }

//============================= INQUIRY    ===================================
bool
Vertex::hasRestrictions() const
{ return mHasRestrictions; }

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////





