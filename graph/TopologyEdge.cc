/*
 * TopologyEdge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "TopologyEdge.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
TopologyEdge::TopologyEdge(
		EdgeId id,
		const TopologyVertex& source,
		const TopologyVertex& target)
	: mId(id), mrSource(source), mrTarget(target)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const TopologyEdge& rEdge)
{
	os  << "TopologyEdge [id: " << rEdge.id()
		<< ", source: " << rEdge.source().mId
		<< ", target: " << rEdge.target().mId << "]";
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

const TopologyVertex&
TopologyEdge::source() const
{ return mrSource; }

const TopologyVertex&
TopologyEdge::target() const
{ return mrTarget; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



