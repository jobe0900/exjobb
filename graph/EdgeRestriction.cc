/*
 * EdgeRestriction.cc
 *
 *      Author: Jonas Bergman
 */

#include "EdgeRestriction.h"  // class implemented

// STATIC INITIALIZATION
/*static*/ double EdgeRestriction::VehicleProperties::DEFAULT_DIMENSION_MAX
    = std::numeric_limits<double>::max();

/*static*/ unsigned EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MAX
    = std::numeric_limits<unsigned>::max();

/*static*/ unsigned EdgeRestriction::VehicleProperties::DEFAULT_SPEED_MIN
    = 0;

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
EdgeRestriction::~EdgeRestriction()
{
    delete mpVehicleProperties;
    delete mpGeneralAccess;
    delete mpBarrier;
    for(auto it : mVehicleTypeAccessMap)
    {
        delete it.second;
    }
    for(auto it : mTurningRestrictions)
    {
        delete it;
    }
}
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
void
EdgeRestriction::setVehiclePropertyRestriction(
    EdgeRestriction::VehicleProperties* pVehicleProperties)
{
    delete mpVehicleProperties;
    mpVehicleProperties = pVehicleProperties;
}

void
EdgeRestriction::setGeneralAccessRestriction(
    OsmAccess*  pGeneralAccess)
{
    delete mpGeneralAccess;
    mpGeneralAccess = pGeneralAccess;
}

void
EdgeRestriction::setGeneralAccessRestriction(
    OsmAccess::AccessType  generalAccessType)
{
    delete mpGeneralAccess;
    mpGeneralAccess = new OsmAccess(generalAccessType);
}

void
EdgeRestriction::addVehicleTypeAccessRestriction(
    OsmVehicle::VehicleType  vehicleType,
    OsmAccess*               pAccess)
{
    if(hasVehicleTypeAccessRestriction(vehicleType))
    {
        auto old_access = mVehicleTypeAccessMap.find(vehicleType);
        delete old_access->second;
        mVehicleTypeAccessMap.erase(vehicleType);
    }
    mVehicleTypeAccessMap.insert({vehicleType, pAccess});
}

void
EdgeRestriction::addVehicleTypeAccessRestriction(
    OsmVehicle::VehicleType  vehicleType,
    OsmAccess::AccessType    accessType)
{
    addVehicleTypeAccessRestriction(vehicleType, new OsmAccess(accessType));
//    if(hasVehicleTypeAccessRestriction(vehicleType))
//    {
//        auto old_access = mVehicleTypeAccessMap.find(vehicleType);
//        delete old_access->second;
//        mVehicleTypeAccessMap.erase(vehicleType);
//    }
//    mVehicleTypeAccessMap.insert({vehicleType, new OsmAccess(accessType)});
}

void
EdgeRestriction::setBarrierRestriction(
    OsmBarrier* pBarrier)
{
    delete mpBarrier;
    mpBarrier = pBarrier;
}

void
EdgeRestriction::setBarrierRestriction(
    OsmBarrier::BarrierType barrierType)
{
    delete mpBarrier;
    mpBarrier = new OsmBarrier(barrierType);
}

void
EdgeRestriction::addTurningRestriction(
    OsmTurningRestriction*   pTurningRestriction)
{
    mTurningRestrictions.push_back(pTurningRestriction);
}

void
EdgeRestriction::setDisusedRestriction()
{
    mIsDisusedEdge = true;
}

void
EdgeRestriction::setNoExitRestriction()
{
    mIsNoExitEdge = true;
}

void
EdgeRestriction::setViaWayRestriction()
{
    mHasViaWayRestriction = true;
}
//============================= ACESS      ===================================
std::vector<EdgeRestriction::RestrictionType>
EdgeRestriction::restrictionTypes() const
{
    std::vector<EdgeRestriction::RestrictionType> rest_types;

    for(int i = EdgeRestriction::VEHICLE_PROPERTIES;
        i < EdgeRestriction::NR_RESTRICTION_TYPES;
        ++i)
    {
        RestrictionType type = static_cast<RestrictionType>(i);
        if(hasRestriction(type))
        {
            rest_types.push_back(type);
        }
    }

    return rest_types;
}

const EdgeRestriction::VehicleProperties&
EdgeRestriction::vehicleProperties() const
{
    if(!hasVehiclePropertyRestriction())
    {
        throw RestrictionsException(
            "Restrictions:vehicleProperties: no restriction for edge");
    }
    return *mpVehicleProperties;
}

EdgeRestriction::VehicleProperties&
EdgeRestriction::vehicleProperties()
{
    if(!hasVehiclePropertyRestriction())
    {
        throw RestrictionsException(
            "Restrictions:vehicleProperties: no restriction for edge");
    }
    return *mpVehicleProperties;
//    return const_cast<EdgeRestriction::VehicleProperties&>(
//        static_cast<const EdgeRestriction&>(*this).vehicleProperties() );
}

unsigned
EdgeRestriction::maxSpeed() const
{
    if(hasVehiclePropertyRestriction())
    {
        return mpVehicleProperties->maxSpeed;
    }
    return VehicleProperties::DEFAULT_SPEED_MAX;
}

//const std::map<EdgeIdType, EdgeRestrictions::VehicleProperties>&
//EdgeRestriction::vehicleProperties() const
//{
//    return mVehiclePropertiesMap;
//}

const OsmAccess&
EdgeRestriction::generalAccess() const
{
    if(!hasGeneralAccessRestriction())
    {
        throw RestrictionsException(
            "Restrictions:generalAccess: no restriction for edge");
    }
    return *mpGeneralAccess;
}

OsmAccess&
EdgeRestriction::generalAccess()
{
    if(!hasGeneralAccessRestriction())
    {
        throw RestrictionsException(
            "Restrictions:generalAccess: no restriction for edge");
    }
    return *mpGeneralAccess;
//    return const_cast<OsmAccess&>(
//        static_cast<const EdgeRestrictions&>(*this).generalAccess(edgeId) );
}

//const std::map<EdgeIdType, OsmAccess>&
//EdgeRestrictions::generalAccess() const
//{
//    return mGeneralAccessMap;
//}

const OsmAccess&
EdgeRestriction::vehicleTypeAccess(
    OsmVehicle::VehicleType vehicleType) const
{
    if(!hasVehicleTypeAccessRestriction(vehicleType))
    {
        throw RestrictionsException(
            "Restrictions:vehicleTypeAccess: no restriction for"
            + " vehicle type " + OsmVehicle::toString(vehicleType))
            + "  for edge";
    }
    return mVehicleTypeAccessMap.find(vehicleType)->second;
}

OsmAccess&
EdgeRestriction::vehicleTypeAccess(
    OsmVehicle::VehicleType vehicleType)
{
    return const_cast<OsmAccess&>
        (static_cast<const EdgeRestriction&>
            (*this).vehicleTypeAccess(vehicleType)
        );
}

std::vector<OsmVehicle::VehicleType>
EdgeRestriction::vehicleTypesWithRestrictions() const
{
    std::vector<OsmVehicle::VehicleType> types;

    for( int i = 0; i < OsmVehicle::NR_VEHICLE_TYPES; ++i)
    {
        OsmVehicle::VehicleType type = static_cast<OsmVehicle::VehicleType>(i);

        if(hasVehicleTypeAccessRestriction(type))
        {
            types.push_back(type);
        }
    }

    return types;
}

//const std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >&
//EdgeRestrictions::vehicleTypeAccessEdges() const
//{
//    return mVehicleTypeAccessMap;
//}

const OsmBarrier&
EdgeRestriction::barrier() const
{
    if(!hasBarrierRestriction())
    {
        throw RestrictionsException(
            "Restrictions:barrier: no restriction for edge");
    }
    return *mpBarrier;
}

//const std::map<EdgeIdType, OsmBarrier>&
//EdgeRestrictions::barriers() const
//{
//    return mBarrierMap;
//}

const std::vector<OsmTurningRestriction*>&
EdgeRestriction::turningRestrictions() const
{
    if(!hasTurningRestriction())
    {
        throw RestrictionsException(
            "Restriction:turningRestriction: no turning restriction for edge");
    }
    return mTurningRestrictions;
}

std::vector<EdgeIdType>
EdgeRestriction::restrictedTargetEdges() const
{
    std::vector<EdgeIdType> restricted_targets;

    try
    {
        const auto& r_vec = this->turningRestrictions();

        for(const auto& restr : r_vec)
        {
            restricted_targets.push_back(restr->toEdgeId());
        }
    }
    catch (RestrictionsException& re)
    {
        // never mind
    }
    return restricted_targets;
}

//const std::set<EdgeIdType>&
//EdgeRestriction::disusedEdges() const
//{
//    return mDisusedEdges;
//}
//
//const std::set<EdgeIdType>&
//EdgeRestrictions::noExitEdges() const
//{
//    return mNoExitEdges;
//}

bool
EdgeRestriction::isEdgeRestricted(
        const VehicleConfig& rVehicleConfig,
        const OsmBarrier::RestrictionsRule& rBarrierRule,
        const OsmAccess::AccessRule& rAccessRule) const
{
    const auto& restriction_types = restrictionTypes();

    bool is_restricted = false;
    bool is_generally_restricted = false;
    bool is_vehicle_banned = false;

    for(const auto& r : restriction_types)
    {
        switch (r)
        {
            case EdgeRestrictions::DISUSED:
                is_restricted = true; break;
            case EdgeRestrictions::VEHICLE_PROPERTIES:
                if(vehicleProperties().restrictsAccess(rVehicleConfig))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestrictions::BARRIER:
                if(barrier().restrictsAccess(rBarrierRule))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestrictions::GENERAL_ACCESS:
                if(!generalAccess().allowsAccess(rAccessRule))
                {
                    is_generally_restricted = true;
                }
                continue;
            case EdgeRestrictions::VEHICLE_TYPE_ACCESS:
                if(!vehicleTypeAccess(rVehicleConfig.category)
                    .allowsAccess(rAccessRule))
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
        return true; // this edge should not be added.
    }
    return false;
}

//============================= INQUIRY    ===================================
bool
EdgeRestriction::hasRestriction(
    EdgeRestriction::RestrictionType type) const
{
    switch (type)
    {
        case VEHICLE_PROPERTIES:
            return hasVehiclePropertyRestriction(); break;
        case GENERAL_ACCESS:
            return hasGeneralAccessRestriction(); break;
        case VEHICLE_TYPE_ACCESS:
            return hasVehicleTypeAccessRestriction(); break;
        case BARRIER:
            return hasBarrierRestriction(); break;
        case TURNING:
            return hasTurningRestriction(); break;
        case DISUSED:
            return hasDisusedRestriction(); break;
        case NO_EXIT:
            return hasNoExitRestriction(); break;
        default:
            return false;
    }
}

bool
EdgeRestriction::hasVehiclePropertyRestriction() const
{
    return mpVehicleProperties != nullptr;
}

bool
EdgeRestriction::hasMaxSpeedRestriction() const
{
    if(hasVehiclePropertyRestriction())
    {
        return mpVehicleProperties->maxSpeed != VehicleProperties::DEFAULT_SPEED_MAX;
    }
    return false;
}

//bool
//EdgeRestriction::hasGeneralAccessRestriction(EdgeIdType edgeId) const
//{
//    auto it = mGeneralAccessMap.find(edgeId);
//    return (it != mGeneralAccessMap.end());
//}
//
//bool
//EdgeRestrictions::hasVehicleTypeAccessRestriction(EdgeIdType edgeId) const
//{
//    auto it = mVehicleTypeAccessMap.find(edgeId);
//    return (it != mVehicleTypeAccessMap.end());
//}

bool
EdgeRestriction::hasVehicleTypeAccessRestriction(
    OsmVehicle::VehicleType vehicleType) const
{
    auto it = mVehicleTypeAccessMap.find(vehicleType);
    if (it != mVehicleTypeAccessMap.end())
    {
        return true;
    }
    return false;
}

bool
EdgeRestriction::hasBarrierRestriction() const
{
    return  mpBarrier != nullptr;
}

bool
EdgeRestriction::hasTurningRestriction() const
{
    return mTurningRestrictions.size() > 0;
}

bool
EdgeRestriction::hasDisusedRestriction() const
{
    return mIsDisusedEdge;
}

bool
EdgeRestriction::hasNoExitRestriction() const
{
    return mIsNoExitEdge;
}

bool
EdgeRestriction::hasViaWayRestriction() const
{
    return mHasViaWayRestriction;
}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


