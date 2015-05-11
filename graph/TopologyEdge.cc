/*
 * TopologyEdge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "TopologyEdge.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
TopologyEdge::TopologyEdge(EdgeId id, VertexId source, VertexId target)
	: mId(id), mSource(source), mTarget(target)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const TopologyEdge& rEdge)
{
	os  << "TopologyEdge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target() << "]";
	return os;
}

bool
TopologyEdge::operator==(const TopologyEdge& rhs) const
{
	return (rhs.id() == id())
			&& (rhs.source() == source())
			&& (rhs.target() == target());
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
EdgeId
TopologyEdge::id() const
{ return mId; }

VertexId
TopologyEdge::source() const
{ return mSource; }

VertexId
TopologyEdge::target() const
{ return mTarget; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



