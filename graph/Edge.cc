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
	: mId(id), mSource(source), mTarget(target), mpEdgeData(nullptr)
{}

Edge::Edge(const Edge& from)
{
    mId = from.mId;
    mSource = from.mSource;
    mTarget = from.mTarget;

    // TODO use shared_ptr instead?
    mpEdgeData = nullptr;

    // make a copy of the
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

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



