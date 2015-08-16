/*
 * OsmBarrier.cc
 *
 *  Created on: 2015-05-25
 *      Author: Jonas Bergman
 */


#include "OsmBarrier.h"  // class implemented

// RestrictionsRule ----------------------------------------------------------
OsmBarrier::RestrictionsRule::RestrictionsRule(
    std::initializer_list<OsmBarrier::BarrierType> restrictionTypes)
    : restrictionTypes(restrictionTypes)
{ }

bool
OsmBarrier::RestrictionsRule::restrictsAccess(OsmBarrier::BarrierType type) const
{
    auto it = std::find(restrictionTypes.begin(), restrictionTypes.end(), type);
    return it != restrictionTypes.end();
}

bool
OsmBarrier::CostsRule::costsToPass(OsmBarrier::BarrierType type) const
{
    const auto& it  = costs.find(type);
    return it != costs.end();
}

Cost
OsmBarrier::CostsRule::getCost(OsmBarrier::BarrierType type) const
{
    const auto& it  = costs.find(type);
    if(it != costs.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

void
OsmBarrier::CostsRule::addCost(OsmBarrier::BarrierType type, Cost cost)
{
    costs.erase(type);
    costs.insert({type, cost});
}

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
OsmBarrier::OsmBarrier(OsmBarrier::BarrierType type)
    : mType(type)
{}

//============================= OPERATORS ====================================

//static
OsmBarrier::BarrierType
OsmBarrier::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < sTypeStrings.size(); ++i)
    {
        if(rTypeString == OsmBarrier::sTypeStrings[i])
        {
            return static_cast<BarrierType>(i);
        }
    }
    // no match in types. Look if it is disregarded or unknown.
    auto it = std::find(sDisregardedTypes.begin(),
                        sDisregardedTypes.end(),
                        rTypeString);
    if(it != sDisregardedTypes.end())
    {
        return BarrierType::NONE;
    }
    throw OsmException("OsmBarrier:parseString: Unknown Barrier Type string: "
        + rTypeString);
}

//static
std::string
OsmBarrier::toString(OsmBarrier::BarrierType accessType)
{
    if(accessType >= sTypeStrings.size())
    {
        throw OsmException("OsmBarrier:toString: Unknown Barrier Type");
    }
    return OsmBarrier::sTypeStrings[accessType];
}

std::string
OsmBarrier::toString() const
{
    return sTypeStrings[this->mType];
}

bool
OsmBarrier::restrictsAccess(OsmBarrier::RestrictionsRule rule) const
{
    return rule.restrictsAccess(mType);
}

bool
OsmBarrier::costsToPass(OsmBarrier::CostsRule rule) const
{
    return rule.costsToPass(mType);
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const std::vector<std::string> OsmBarrier::sTypeStrings
{
    "none",
    "block",
    "bollard",
    "border_control",
    "bump_gate",
    "bus_trap",
    "cattle_grid",
    "chain",
    "cycle_barrier",
    "debris",
    "entrance",
    "full-height_turnstile",
    "gate",
    "hampshire_gate",
    "height_restrictor",
    "horse_stile",
    "jersey_barrier",
    "kent_carriage_gap",
    "kissing_gate",
    "lift_gate",
    "log",
    "motorcycle_barrier",
    "rope",
    "sally_port",
    "spikes",
    "stile",
    "sump_buster",
    "swing_gate",
    "toll_booth",
    "turnstile",
    "yes"
};

const std::vector<std::string> OsmBarrier::sDisregardedTypes
{
    "cable_barrier",
    "city_wall",
    "ditch",
    "fence",
    "guard_rail",
    "handrail",
    "hedge",
    "kerb",
    "retaining_wall",
    "wall",
};
