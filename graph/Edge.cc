/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented

//============================= TYPES     ====================================
const EdgeIdType Edge::MAX_ID = std::numeric_limits<EdgeIdType>::max();

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
    os << ", type: " << OsmHighway::toString(roadType);
}


// Edge ----------------------------------------------------------------------
/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Edge::Edge(EdgeIdType       id,
           OsmIdType        osmId,
           VertexIdType     source,
           VertexIdType     target,
           Edge::GeomData   geomData,
           Edge::RoadData   roadData)
    : mId(id),
      mOsmId(osmId),
      mSource(source),
      mTarget(target),
      mGeomData(geomData),
      mRoadData(roadData),
      mpRestrictions(nullptr)
//      mHasRestrictions(false),
//      mHasViaWayRestriction(false)
{ }

Edge::Edge(EdgeIdType       id,
           OsmIdType        osmId,
           VertexIdType     source,
           VertexIdType     target)
    : mId(id),
      mOsmId(osmId),
      mSource(source),
      mTarget(target),
      mGeomData(),
      mRoadData(),
      mpRestrictions(nullptr)
//      mHasRestrictions(false),
//      mHasViaWayRestriction(false)
{ }

Edge::~Edge()
{
    delete mpRestrictions;
}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Edge& rEdge)
{
	os  << "Edge [id: " << rEdge.id()
	    << ", osmId: " << rEdge.osmId()
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

void
Edge::setRestrictions(EdgeRestriction* pRestrictions)
{
    delete mpRestrictions;
    mpRestrictions = pRestrictions;
}

void
Edge::clearRestrictions()
{
    delete mpRestrictions;
    mpRestrictions = nullptr;
}

//void
//Edge::setHasRestrictions(bool hasRestrictions)
//{
//    mHasRestrictions = hasRestrictions;
//}
//
//void
//Edge::setHasViaWayRestriction(bool hasViaWayRestriction)
//{
//    mHasViaWayRestriction = hasViaWayRestriction;
//}

//static
EdgeIdType
Edge::parse(const std::string& idStr)
{
    return static_cast<EdgeIdType>(std::stoul(idStr));
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

OsmIdType
Edge::osmId() const
{ return mOsmId; }

const Edge::GeomData&
Edge::geomData() const
{ return mGeomData; }

const Edge::RoadData&
Edge::roadData() const
{ return mRoadData; }

EdgeRestriction*
Edge::getRestrictions()
{
    if(!hasRestrictions()) {
        mpRestrictions = new EdgeRestriction();
    }
    return mpRestrictions;
}

//============================= INQUIRY    ===================================
bool
Edge::hasRestrictions() const
{ return mpRestrictions != nullptr; }

bool
Edge::hasViaWayRestriction() const
{
    if(hasRestrictions())
    {
        return mpRestrictions->mHasViaWayRestriction();
    }
    return false;
}

//bool
//Edge::hasMaxSpeedRestriction() const
//{
//    if(hasRestrictions())
//    {
//        return mpRestrictions->hasMaxSpeedRestriction();
//    }
//    return false;
//}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////



