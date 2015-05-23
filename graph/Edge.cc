/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented

//============================= TYPES     ====================================
// Edge::GeomData ------------------------------------------------------------
Edge::GeomData::GeomData(double length,
                         Point  centerPoint,
                         int    sourceBearing,
                         int    targetBearing)
    : length(length),
      centerPoint(centerPoint),
      sourceBearing(sourceBearing),
      targetBearing(targetBearing)
{}

// Edge::RoadData ------------------------------------------------------------
Edge::RoadData::RoadData(DirectionType direction, size_t nrLanes)
    : direction(direction), nrLanes(nrLanes)
{}

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


// Edge ----------------------------------------------------------------------
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

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "Edge [id: " << rEdge.id()
		<< ", source: " << rEdge.source()
		<< ", target: " << rEdge.target()
		<< "\n   road data: ";
	rEdge.roadData().print(os);

	os  << "]";

	return os;
}


//============================= OPERATIONS ===================================

void
Edge::setGeomData(Edge::GeomData geomData)
{ mGeomData = geomData; }

void
Edge::setRoadData(Edge::RoadData roadData)
{ mRoadData = roadData; }

void
Edge::setOsmId(OsmIdType osmId)
{ mOsmId = osmId; }

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

OsmIdType
Edge::osmId() const
{ return mOsmId; }

const Edge::GeomData&
Edge::geomData() const
{ return mGeomData; }

const Edge::RoadData&
Edge::roadData() const
{ return mRoadData; }

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



