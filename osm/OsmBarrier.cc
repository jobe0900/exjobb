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

// CostsRule -----------------------------------------------------------------
OsmBarrier::CostsRule::CostsRule(
    std::initializer_list<OsmBarrier::BarrierType> costsTypes)
    : costsTypes(costsTypes)
{ }

bool
OsmBarrier::CostsRule::costs(OsmBarrier::BarrierType type) const
{
    auto it = std::find(costsTypes.begin(), costsTypes.end(), type);
    return it != costsTypes.end();
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
    throw OsmException("OsmBarrier:parseString: Unknown Barrier Type string.");
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
    return rule.costs(mType);
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






