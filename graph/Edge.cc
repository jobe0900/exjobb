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

Edge::Edge(Edge&& from)
    :   mId(from.mId),
        mOsmId(from.mOsmId),
        mSource(from.mSource),
        mTarget(from.mTarget),
        mGeomData(from.mGeomData),
        mRoadData(from.mRoadData),
        mpRestrictions(from.mpRestrictions)
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

//EdgeRestriction*
//Edge::getRestrictions()
//{
//    if(!hasRestrictions()) {
//        mpRestrictions = new EdgeRestriction();
//    }
//    return mpRestrictions;
//}

EdgeRestriction&
Edge::restrictions()
{
    if(mpRestrictions == nullptr) {
//        throw RestrictionsException(std::string("No restriction on edge ")
//            + std::to_string(mId));
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

//const EdgeRestriction&
//Edge::restrictions() const
//{
//    if(!hasRestrictions()) {
//        mpRestrictions = new EdgeRestriction();
//    }
//    return *mpRestrictions;
//}

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
    const auto& restriction_types = mpRestrictions->restrictionTypes();

    bool is_restricted = false;
    bool is_generally_restricted = false;
    bool is_vehicle_banned = false;

    for(const auto& r : restriction_types)
    {
        switch (r)
        {
            case EdgeRestriction::DISUSED:
                is_restricted = true; break;
            case EdgeRestriction::VEHICLE_PROPERTIES:
                if(mpRestrictions->vehicleProperties()
                    .restrictsAccess(rConfig.getVehicleConfig()))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestriction::BARRIER:
                if(mpRestrictions->barrier()
                    .restrictsAccess(rConfig.getBarrierRestrictionsRule()))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestriction::GENERAL_ACCESS:
                if(!mpRestrictions->generalAccess()
                    .allowsAccess(rConfig.getAccessRule()))
                {
                    is_generally_restricted = true;
                }
                continue;
            case EdgeRestriction::VEHICLE_TYPE_ACCESS:
                if(!mpRestrictions->vehicleTypeAccess(rConfig.getVehicleConfig().category)
                    .allowsAccess(rConfig.getAccessRule()))
                {
                    is_vehicle_banned = true;
                }
                continue;
            default:
                continue;
        }
    }

    if(is_restricted
        || (is_generally_restricted && is_vehicle_banned)
        || is_vehicle_banned)
    {
        return true;
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



