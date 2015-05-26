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
Restrictions::setVehiclePropertyRestricionForEdge(
    EdgeIdType edgeId,
    Restrictions::VehicleProperties vehicleProperties)
{
    if(hasVehiclePropertyRestriction(edgeId))
    {
        mVehiclePropertiesMap.erase(edgeId);
    }
    mVehiclePropertiesMap.insert({edgeId, vehicleProperties});
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
    if(!hasVehiclePropertyRestriction(edgeId))
    {
        throw RestrictionsException(
            "Restrictions:vehicleProperties: no restriction for edge id: "
            + std::to_string(edgeId));
    }
    return mVehiclePropertiesMap.find(edgeId)->second;
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

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


