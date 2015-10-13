/*
 * Edge.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Edge.h"  // class implemented

#include "EdgeRestriction.h"

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
      mSourceId(source),
      mTargetId(target),
      mGeomData(geomData),
      mRoadData(roadData),
      mpRestrictions(nullptr),
      mCost(),
      mSpeed()
{ }

Edge::Edge(EdgeIdType       id,
           OsmIdType        osmId,
           VertexIdType     source,
           VertexIdType     target)
    : mId(id),
      mOsmId(osmId),
      mSourceId(source),
      mTargetId(target),
      mGeomData(),
      mRoadData(),
      mpRestrictions(nullptr),
      mCost(),
      mSpeed()
{ }

Edge::Edge(Edge&& from)
    : mId(from.mId),
      mOsmId(from.mOsmId),
      mSourceId(from.mSourceId),
      mTargetId(from.mTargetId),
      mGeomData(from.mGeomData),
      mRoadData(from.mRoadData),
      mpRestrictions(from.mpRestrictions),
      mCost(),
      mSpeed()
{
    from.mpRestrictions = nullptr;
}

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
        << ", source: " << rEdge.sourceId()
        << ", target: " << rEdge.targetId()
        << ", cost: " << rEdge.cost()
        << ", length: " << rEdge.geomData().length
        << ", speed: " << rEdge.speed()
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
Edge::setSpeed(Speed speed)
{
    mSpeed = speed;
}

void
Edge::clearCostsAndRestrictions()
{
    mCost.clearCosts();

    delete mpRestrictions;
    mpRestrictions = nullptr;

    mSpeed = 0;
}

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
Edge::sourceId() const
{ return mSourceId; }

VertexIdType
Edge::targetId() const
{ return mTargetId; }

OsmIdType
Edge::osmId() const
{ return mOsmId; }

const Edge::GeomData&
Edge::geomData() const
{ return mGeomData; }

const Edge::RoadData&
Edge::roadData() const
{ return mRoadData; }


EdgeRestriction&
Edge::restrictions()
{
    if(mpRestrictions == nullptr) {
        mpRestrictions = new EdgeRestriction();
    }
    return *mpRestrictions;
}

const EdgeRestriction&
Edge::restrictions() const
{
    if(mpRestrictions == nullptr) {
        throw RestrictionsException(std::string("No restriction on edge ")
            + std::to_string(mId));
    }
    return *mpRestrictions;
}

EdgeCost&
Edge::edgeCost()
{
    return mCost;
}

const EdgeCost&
Edge::edgeCost() const
{
    return mCost;
}

Cost
Edge::cost() const
{
    return mCost.getCost();
}

Speed
Edge::speed() const
{
    return mSpeed;
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
        return mpRestrictions->hasViaWayRestriction();
    }
    return false;
}

bool
Edge::isRestricted(const Configuration& rConfig) const
{
    if(mpRestrictions == nullptr)
    {
        return false;
    }

    try
    {
        return mpRestrictions->restricts(rConfig);
    }
    catch (RestrictionsException& re)
    {
        re.addEdgeId(std::to_string(mId));
        throw re;
    }
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

