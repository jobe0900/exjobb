/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented

#include "TopoEdgeData.h"

#include <typeinfo>


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeIdType   id,
           VertexIdType source,
           VertexIdType target)
//           Direction    direction)
	: mId(id), mSource(source), mTarget(target), mpEdgeData(nullptr)
{}

Edge::Edge(const Edge& from)
{
    mId = from.mId;
    mSource = from.mSource;
    mTarget = from.mTarget;
    mpEdgeData = nullptr;
    if((from.mpEdgeData != nullptr)
    && (typeid(TopoEdgeData) == typeid(*from.edgeData())))
    {
        const TopoEdgeData& ted = *(static_cast<TopoEdgeData*>(from.edgeData()));
        mpEdgeData = new TopoEdgeData(ted);
    }
}

Edge::~Edge()
{
    delete mpEdgeData;
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "Edge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target();

	if(rEdge.mpEdgeData != nullptr)
	{
		os << ", edgeData: " << *rEdge.mpEdgeData;
	}

//	switch(rEdge.mDirection)
//	{
//	    case Edge::BOTH:
//	        os << "BOTH"; break;
//	    case Edge::FROM_TO:
//	        os << "FROM_TO"; break;
//	    case Edge::TO_FROM:
//	        os << "TO_FROM"; break;
//	}

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

void
Edge::setEdgeData(EdgeData* pEdgeData)
{
    mpEdgeData = pEdgeData;
}

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

EdgeData*
Edge::edgeData() const
{ return mpEdgeData; }

//Edge::Direction
//Edge::direction() const
//{ return mDirection; }
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



