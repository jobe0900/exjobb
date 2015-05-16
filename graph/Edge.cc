/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeIdType   id,
           VertexIdType source,
           VertexIdType target,
           Direction    direction)
	: mId(id), mSource(source), mTarget(target), mDirection(direction)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "Edge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target()
		<< ", direction: ";

	switch(rEdge.mDirection)
	{
	    case Edge::BOTH:
	        os << "BOTH"; break;
	    case Edge::FROM_TO:
	        os << "FROM_TO"; break;
	    case Edge::TO_FROM:
	        os << "TO_FROM"; break;
	}

	os  << "]";

	return os;
}

bool
Edge::operator==(const Edge& rhs) const
{
	return (rhs.id() == id())
			&& (rhs.source() == source())
			&& (rhs.target() == target());
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
EdgeIdType
Edge::id() const
{ return mId; }

VertexIdType
Edge::source() const
{ return mSource; }

VertexIdType
Edge::target() const
{ return mTarget; }

Edge::Direction
Edge::direction() const
{ return mDirection; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



