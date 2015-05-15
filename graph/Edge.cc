/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeId id, VertexId source, VertexId target)
	: mId(id), mSource(source), mTarget(target)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "TopologyEdge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target() << "]";
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
EdgeId
Edge::id() const
{ return mId; }

VertexId
Edge::source() const
{ return mSource; }

VertexId
Edge::target() const
{ return mTarget; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



