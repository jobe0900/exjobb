/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented

//#include "TopoEdgeData.h"

//#include <typeinfo>


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeIdType       id,
           VertexIdType     source,
           VertexIdType     target,
           Edge::GeomData   geomData,
           Edge::RoadData   roadData)
    : mId(id),
      mSource(source),
      mTarget(target),
      mGeomData(geomData),
      mRoadData(roadData)
{ }

Edge::Edge(EdgeIdType       id,
           VertexIdType     source,
           VertexIdType     target)
    : mId(id),
      mSource(source),
      mTarget(target),
      mGeomData(),
      mRoadData()
{ }
//Edge::Edge(EdgeIdType   id,
//           VertexIdType source,
//           VertexIdType target)
//	: mId(id), mSource(source), mTarget(target), mpEdgeData(nullptr)
//{}

//Edge::Edge(const Edge& from)
//{
//    mId = from.mId;
//    mSource = from.mSource;
//    mTarget = from.mTarget;
//
//    // TODO use shared_ptr instead?
//    mpEdgeData = nullptr;
//
//    // make a copy of the
//    if((from.mpEdgeData != nullptr)
//    && (typeid(TopoEdgeData) == typeid(*from.edgeData())))
//    {
//        const TopoEdgeData& ted = *(static_cast<TopoEdgeData*>(from.edgeData()));
//        mpEdgeData = new TopoEdgeData(ted);
//    }
//}
//
//Edge::~Edge()
//{
//    delete mpEdgeData;
//}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "Edge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target()
		<< "\n   road data: ";
	rEdge.roadData().print(os);

//	if(rEdge.mpEdgeData != nullptr)
//	{
//		os << ", edgeData: " << *rEdge.mpEdgeData;
//	}

	os  << "]";

	return os;
}

//bool
//Edge::operator==(const Edge& rhs) const
//{
//	return (rhs.id() == id())
//			&& (rhs.source() == source())
//			&& (rhs.target() == target())
//			&& (rhs.geomData() == geomData())
//			&& (rhs.roadData() == roadData());
//}

//============================= OPERATIONS ===================================

void
Edge::setGeomData(Edge::GeomData geomData)
{ mGeomData = geomData; }

void
Edge::setRoadData(Edge::RoadData roadData)
{ mRoadData = roadData; }

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

const Edge::GeomData&
Edge::geomData() const
{ return mGeomData; }

const Edge::RoadData&
Edge::roadData() const
{ return mRoadData; }

//EdgeData*
//Edge::edgeData() const
//{ return mpEdgeData; }

//============================= INQUIRY    ===================================

//============================= TYPES      ===================================
void
Edge::RoadData::print(std::ostream& os) const
{
    os  << "direction: ";

    switch(direction)
    {
        case Edge::DirectionType::BOTH:
            os << "BOTH"; break;
        case Edge::DirectionType::FROM_TO:
            os << "FROM_TO"; break;
        case Edge::DirectionType::TO_FROM:
            os << "TO_FROM"; break;
    }

    os << ", #lanes: " << nrLanes;
}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



