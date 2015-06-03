/*
 * EdgeRestrictions.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "EdgeRestrictions.h"  // class implemented

// STATIC INITIALIZATION
/*static*/ double EdgeRestrictions::VehicleProperties::DEFAULT_DIMENSION_MAX
    = std::numeric_limits<double>::max();

/*static*/ unsigned EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MAX
    = std::numeric_limits<unsigned>::max();

/*static*/ unsigned EdgeRestrictions::VehicleProperties::DEFAULT_SPEED_MIN
    = 0;

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
//EdgeRestrictions::EdgeRestrictions()
//    : mVehiclePropertiesMap(),
//      mGeneralAccessMap(),
//      mVehicleTypeAccessMap(),
//      mBarrierMap(),
//      mTurningRestrictionsMap(),
//      mDisusedEdges(),
//      mNoExitEdges()
//{ }

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
void
EdgeRestrictions::setVehiclePropertyRestrictionForEdge(
    EdgeIdType edgeId,
    EdgeRestrictions::VehicleProperties vehicleProperties)
{
    if(hasVehiclePropertyRestriction(edgeId))
    {
        mVehiclePropertiesMap.erase(edgeId);
    }
    mVehiclePropertiesMap.insert({edgeId, vehicleProperties});
}

void
EdgeRestrictions::setGeneralAccessRestrictionForEdge(
    EdgeIdType edgeId,
    OsmAccess  generalAccess)
{
    if(hasGeneralAccessRestriction(edgeId))
    {
        mGeneralAccessMap.erase(edgeId);
    }
    mGeneralAccessMap.insert({edgeId, generalAccess});
}

void
EdgeRestrictions::setGeneralAccessRestrictionForEdge(
    EdgeIdType             edgeId,
    OsmAccess::AccessType  generalAccessType)
{
    if(hasGeneralAccessRestriction(edgeId))
    {
        mGeneralAccessMap.erase(edgeId);
    }
    mGeneralAccessMap.insert({edgeId, OsmAccess(generalAccessType)});
}

void
EdgeRestrictions::addVehicleTypeAccessRestrictionsForEdge(
    EdgeIdType               edgeId,
    OsmVehicle::VehicleType  vehicleType,
    OsmAccess                access)
{

    if(hasVehicleTypeAccessRestriction(edgeId))
    {
        auto& vehicle_map = mVehicleTypeAccessMap.find(edgeId)->second;
        if(hasVehicleTypeAccessRestriction(edgeId, vehicleType))
        {
            vehicle_map.erase(vehicleType);
        }
        vehicle_map.insert({vehicleType, access});
    }
    else
    {
        std::map<OsmVehicle::VehicleType, OsmAccess> vehicle_map;
        vehicle_map.insert({vehicleType, access});
        mVehicleTypeAccessMap.insert({edgeId, vehicle_map});
    }
}

void
EdgeRestrictions::addVehicleTypeAccessRestrictionsForEdge(
    EdgeIdType               edgeId,
    OsmVehicle::VehicleType  vehicleType,
    OsmAccess::AccessType    accessType)
{
    addVehicleTypeAccessRestrictionsForEdge(
        edgeId,
        vehicleType,
        OsmAccess(accessType));
}

void
EdgeRestrictions::setBarrierRestrictionForEdge(
    EdgeIdType edgeId,
    OsmBarrier barrier)
{
    if(hasBarrierRestriction(edgeId))
    {
        mBarrierMap.erase(edgeId);
    }
    mBarrierMap.insert({edgeId, barrier});
}

void
EdgeRestrictions::setBarrierRestrictionForEdge(
    EdgeIdType              edgeId,
    OsmBarrier::BarrierType barrierType)
{
    if(hasBarrierRestriction(edgeId))
    {
        mBarrierMap.erase(edgeId);
    }
    mBarrierMap.insert({edgeId, OsmBarrier(barrierType)});
}

void
EdgeRestrictions::addTurningRestrictionForEdge(
    EdgeIdType              edgeId,
    OsmTurningRestriction   turningRestriction)
{
    if(hasTurningRestriction(edgeId))
    {
        auto& turns = mTurningRestrictionsMap.find(edgeId)->second;
        turns.push_back(turningRestriction);
    }
    else
    {
        std::vector<OsmTurningRestriction> turns;
        turns.push_back(turningRestriction);
        mTurningRestrictionsMap.insert({edgeId, turns});
    }
}

void
EdgeRestrictions::setDisusedRestrictionForEdge(EdgeIdType edgeId)
{
    mDisusedEdges.insert(edgeId);
}

void
EdgeRestrictions::setNoExitRestrictionForEdge(EdgeIdType edgeId)
{
    mNoExitEdges.insert(edgeId);
}
//============================= ACESS      ===================================
std::vector<EdgeRestrictions::RestrictionType>
EdgeRestrictions::restrictionTypes(EdgeIdType edgeId) const
{
    std::vector<EdgeRestrictions::RestrictionType> rest_types;

    for(int i = EdgeRestrictions::VEHICLE_PROPERTIES;
        i < EdgeRestrictions::NR_RESTRICTION_TYPES;
        ++i)
    {
        RestrictionType type = static_cast<RestrictionType>(i);
        if(hasRestriction(edgeId, type))
        {
            rest_types.push_back(type);
        }
    }

    return rest_types;
}

const EdgeRestrictions::VehicleProperties&
EdgeRestrictions::vehicleProperties(EdgeIdType edgeId) const
{
    if(!hasVehiclePropertyRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:vehicleProperties: no restriction for edge id: "
            + std::to_string(edgeId));
    }
    return mVehiclePropertiesMap.find(edgeId)->second;
}

EdgeRestrictions::VehicleProperties&
EdgeRestrictions::vehicleProperties(EdgeIdType edgeId)
{
    return const_cast<EdgeRestrictions::VehicleProperties&>(
        static_cast<const EdgeRestrictions&>(*this).vehicleProperties(edgeId) );
}

const std::map<EdgeIdType, EdgeRestrictions::VehicleProperties>&
EdgeRestrictions::vehicleProperties() const
{
    return mVehiclePropertiesMap;
}

const OsmAccess&
EdgeRestrictions::generalAccess(EdgeIdType edgeId) const
{
    if(!hasGeneralAccessRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:generalAccess: no restriction for edge id:"
            + std::to_string(edgeId));
    }
    return mGeneralAccessMap.find(edgeId)->second;
}

OsmAccess&
EdgeRestrictions::generalAccess(EdgeIdType edgeId)
{
    return const_cast<OsmAccess&>(
        static_cast<const EdgeRestrictions&>(*this).generalAccess(edgeId) );

}

const std::map<EdgeIdType, OsmAccess>&
EdgeRestrictions::generalAccess() const
{
    return mGeneralAccessMap;
}

const OsmAccess&
EdgeRestrictions::vehicleTypeAccess(
    EdgeIdType              edgeId,
    OsmVehicle::VehicleType vehicleType) const
{
    if(!hasVehicleTypeAccessRestriction(edgeId, vehicleType))
    {
        throw RestrictionsException(
            "Restrictions:vehicleTypeAccess: no restriction for edge id:"
            + std::to_string(edgeId)
            + " and vehicle type " + OsmVehicle::toString(vehicleType));
    }
    auto vehicle_map = mVehicleTypeAccessMap.find(edgeId)->second;
    return vehicle_map.find(vehicleType)->second;
}

OsmAccess&
EdgeRestrictions::vehicleTypeAccess(
    EdgeIdType              edgeId,
    OsmVehicle::VehicleType vehicleType)
{
    return const_cast<OsmAccess&>
        (static_cast<const EdgeRestrictions&>
            (*this).vehicleTypeAccess(edgeId, vehicleType)
        );
}

std::vector<OsmVehicle::VehicleType>
EdgeRestrictions::vehicleTypesWithRestrictions(EdgeIdType edgeId) const
{
    std::vector<OsmVehicle::VehicleType> types;

    for( int i = 0; i < OsmVehicle::NR_VEHICLE_TYPES; ++i)
    {
        OsmVehicle::VehicleType type = static_cast<OsmVehicle::VehicleType>(i);

        if(hasVehicleTypeAccessRestriction(edgeId, type))
        {
            types.push_back(type);
        }
    }

    return types;
}

const std::map<EdgeIdType, std::map<OsmVehicle::VehicleType, OsmAccess> >&
EdgeRestrictions::vehicleTypeAccessEdges() const
{
    return mVehicleTypeAccessMap;
}

const OsmBarrier&
EdgeRestrictions::barrier(EdgeIdType edgeId) const
{
    if(!hasBarrierRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:barrier: no restriction for edge id:"
            + std::to_string(edgeId));
    }
    return mBarrierMap.find(edgeId)->second;
}

const std::map<EdgeIdType, OsmBarrier>&
EdgeRestrictions::barriers() const
{
    return mBarrierMap;
}

const std::vector<OsmTurningRestriction>&
EdgeRestrictions::turningRestrictions(EdgeIdType edgeId) const
{
    if(!hasTurningRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restriction:turningRestriction: no turning restriction for "
            "edge id: " + std::to_string(edgeId));
    }
    return mTurningRestrictionsMap.find(edgeId)->second;
}

std::vector<EdgeIdType>
EdgeRestrictions::restrictedTargetEdges(EdgeIdType sourceEdgeId) const
{
    std::vector<EdgeIdType> restricted_targets;

    try
    {
        const auto& r_vec = this->turningRestrictions(sourceEdgeId);

        for(const auto& restr : r_vec)
        {
            restricted_targets.push_back(restr.toEdgeId());
        }
    }
    catch (RestrictionsException& re)
    {
        // never mind
    }
    return restricted_targets;
}

const std::set<EdgeIdType>&
EdgeRestrictions::disusedEdges() const
{
    return mDisusedEdges;
}

const std::set<EdgeIdType>&
EdgeRestrictions::noExitEdges() const
{
    return mNoExitEdges;
}

bool
EdgeRestrictions::isEdgeRestricted(
        EdgeIdType edgeId,
        const VehicleConfig& rVehicleConfig,
        const OsmBarrier::RestrictionsRule& rBarrierRule,
        const OsmAccess::AccessRule& rAccessRule) const
{
    const auto& restriction_types = restrictionTypes(edgeId);

    bool is_restricted = false;
    bool is_generally_restricted = false;
    bool is_vehicle_allowed = false;

    for(const auto& r : restriction_types)
    {
        switch (r)
        {
            case EdgeRestrictions::DISUSED:
                is_restricted = true; break;
            case EdgeRestrictions::VEHICLE_PROPERTIES:
                if(vehicleProperties(edgeId).restrictsAccess(rVehicleConfig))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestrictions::BARRIER:
                if(barrier(edgeId).restrictsAccess(rBarrierRule))
                {
                    is_restricted = true;
                }
                break;
            case EdgeRestrictions::GENERAL_ACCESS:
                if(!generalAccess(edgeId).allowsAccess(rAccessRule))
                {
                    is_generally_restricted = true;
                }
                continue;
            case EdgeRestrictions::VEHICLE_TYPE_ACCESS:
                if(vehicleTypeAccess(edgeId, rVehicleConfig.category)
                    .allowsAccess(rAccessRule))
                {
                    is_vehicle_allowed = true;
                }
                continue;
            default:
                continue;
        }
    }

    if(is_restricted || (is_generally_restricted && !is_vehicle_allowed))
    {
        return true; // this edge should not be added.
    }
    return false;
}

//============================= INQUIRY    ===================================
bool
EdgeRestrictions::hasRestriction(
    EdgeIdType edgeId,
    EdgeRestrictions::RestrictionType type) const
{
    switch (type)
    {
        case VEHICLE_PROPERTIES:
            return hasVehiclePropertyRestriction(edgeId); break;
        case GENERAL_ACCESS:
            return hasGeneralAccessRestriction(edgeId); break;
        case VEHICLE_TYPE_ACCESS:
            return hasVehicleTypeAccessRestriction(edgeId); break;
        case BARRIER:
            return hasBarrierRestriction(edgeId); break;
        case TURNING:
            return hasTurningRestriction(edgeId); break;
        case DISUSED:
            return hasDisusedRestriction(edgeId); break;
        case NO_EXIT:
            return hasNoExitRestriction(edgeId); break;
        default:
            return false;
    }
}

bool
EdgeRestrictions::hasVehiclePropertyRestriction(EdgeIdType edgeId) const
{
    auto it = mVehiclePropertiesMap.find(edgeId);
    return (it != mVehiclePropertiesMap.end());
}

bool
EdgeRestrictions::hasGeneralAccessRestriction(EdgeIdType edgeId) const
{
    auto it = mGeneralAccessMap.find(edgeId);
    return (it != mGeneralAccessMap.end());
}

bool
EdgeRestrictions::hasVehicleTypeAccessRestriction(EdgeIdType edgeId) const
{
    auto it = mVehicleTypeAccessMap.find(edgeId);
    return (it != mVehicleTypeAccessMap.end());
}

bool
EdgeRestrictions::hasVehicleTypeAccessRestriction(
    EdgeIdType              edgeId,
    OsmVehicle::VehicleType vehicleType) const
{
    auto it = mVehicleTypeAccessMap.find(edgeId);
    if (it != mVehicleTypeAccessMap.end())
    {
        const auto& vehicle_map = it->second;
        auto map_it = vehicle_map.find(vehicleType);
        if(map_it != vehicle_map.end())
        {
            return true;
        }
    }
    return false;
}

bool
EdgeRestrictions::hasBarrierRestriction(EdgeIdType edgeId) const
{
    auto it = mBarrierMap.find(edgeId);
    return (it != mBarrierMap.end());
}

bool
EdgeRestrictions::hasTurningRestriction(EdgeIdType edgeId) const
{
    auto it = mTurningRestrictionsMap.find(edgeId);
    return (it != mTurningRestrictionsMap.end());
}

bool
EdgeRestrictions::hasDisusedRestriction(EdgeIdType edgeId) const
{
    auto it = mDisusedEdges.find(edgeId);
    return (it != mDisusedEdges.end());
}

bool
EdgeRestrictions::hasNoExitRestriction(EdgeIdType edgeId) const
{
    auto it = mNoExitEdges.find(edgeId);
    return (it != mNoExitEdges.end());
}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


