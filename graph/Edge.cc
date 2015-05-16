/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeIdType id, VertexIdType source, VertexIdType target)
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
EdgeIdType
Edge::id() const
{ return mId; }

VertexIdType
Edge::source() const
{ return mSource; }

VertexIdType
Edge::target() const
{ return mTarget; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



