/*
 * Restrictions.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "Restrictions.h"  // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
Restrictions::Restrictions()
    : mVehiclePropertiesMap(),
      mGeneralAccessMap(),
      mVehicleTypeAccessMap(),
      mBarrierMap(),
      mTurnRestrictionsMap(),
      mDisusedMap(),
      mNoExitMap()
{ }

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
void
Restrictions::setVehiclePropertyRestrictionForEdge(
    EdgeIdType edgeId,
    Restrictions::VehicleProperties vehicleProperties)
{
    if(hasVehiclePropertyRestriction(edgeId))
    {
        mVehiclePropertiesMap.erase(edgeId);
    }
    mVehiclePropertiesMap.insert({edgeId, vehicleProperties});
}

void
Restrictions::setGeneralAccessRestrictionForEdge(
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
Restrictions::setVehicleTypeAccessRestrictionsForEdge(
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
        mVehicleTypeAccessMap.insert({edgeId, vehicle_map});
    }
    else
    {
        std::map<OsmVehicle::VehicleType, OsmAccess> vehicle_map;
        vehicle_map.insert({vehicleType, access});
        mVehicleTypeAccessMap.insert({edgeId, vehicle_map});
    }
}

void
Restrictions::setBarrierRestrictionForEdge(
    EdgeIdType edgeId,
    OsmBarrier barrier)
{
    if(hasBarrierRestriction(edgeId))
    {
        mBarrierMap.erase(edgeId);
    }
    mBarrierMap.insert({edgeId, barrier});
}
//============================= ACESS      ===================================
std::vector<Restrictions::RestrictionType>
Restrictions::restrictionTypes(EdgeIdType edgeId) const
{
    std::vector<Restrictions::RestrictionType> rest_types;

    for(int i = Restrictions::VEHICLE_PROPERTIES;
        i < Restrictions::NR_RESTRICTION_TYPES;
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

const Restrictions::VehicleProperties&
Restrictions::vehicleProperties(EdgeIdType edgeId) const
{
    if(!hasVehiclePropertyRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:vehicleProperties: no restriction for edge id: "
            + std::to_string(edgeId));
    }
    return mVehiclePropertiesMap.find(edgeId)->second;
}

Restrictions::VehicleProperties&
Restrictions::vehicleProperties(EdgeIdType edgeId)
{
    return const_cast<Restrictions::VehicleProperties&>(
        static_cast<const Restrictions&>(*this).vehicleProperties(edgeId) );
}

const OsmAccess&
Restrictions::generalAccess(EdgeIdType edgeId) const
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
Restrictions::generalAccess(EdgeIdType edgeId)
{
    return const_cast<OsmAccess&>(
        static_cast<const Restrictions&>(*this).generalAccess(edgeId) );

}

const OsmAccess&
Restrictions::vehicleTypeAccess(
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
Restrictions::vehicleTypeAccess(
    EdgeIdType              edgeId,
    OsmVehicle::VehicleType vehicleType)
{
    return const_cast<OsmAccess&>
        (static_cast<const Restrictions&>
            (*this).vehicleTypeAccess(edgeId, vehicleType)
        );
}

std::vector<OsmVehicle::VehicleType>
Restrictions::vehicleTypesWithRestrictions(EdgeIdType edgeId) const
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

const OsmBarrier&
Restrictions::barrier(EdgeIdType edgeId) const
{
    if(!hasBarrierRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:barrier: no restriction for edge id:"
            + std::to_string(edgeId));
    }
    return mBarrierMap.find(edgeId)->second;
}
//============================= INQUIRY    ===================================
bool
Restrictions::hasRestriction(
    EdgeIdType edgeId,
    Restrictions::RestrictionType type) const
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
        default:
            return false;
    }
}

bool
Restrictions::hasVehiclePropertyRestriction(EdgeIdType edgeId) const
{
    auto it = mVehiclePropertiesMap.find(edgeId);
    return (it != mVehiclePropertiesMap.end());
}

bool
Restrictions::hasGeneralAccessRestriction(EdgeIdType edgeId) const
{
    auto it = mGeneralAccessMap.find(edgeId);
    return (it != mGeneralAccessMap.end());
}

bool
Restrictions::hasVehicleTypeAccessRestriction(EdgeIdType edgeId) const
{
    auto it = mVehicleTypeAccessMap.find(edgeId);
    return (it != mVehicleTypeAccessMap.end());
}

bool
Restrictions::hasVehicleTypeAccessRestriction(
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
Restrictions::hasBarrierRestriction(EdgeIdType edgeId) const
{
    auto it = mBarrierMap.find(edgeId);
    return (it != mBarrierMap.end());
}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


