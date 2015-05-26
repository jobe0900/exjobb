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
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


