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
Restrictions::hasVehiclePropertyRestriction(EdgeIdType edgeId) const
{
    auto it = mVehiclePropertiesMap.find(edgeId);
    return (it != mVehiclePropertiesMap.end());
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


