/*
 * OsmAccess.cc
 *
 * @author  Jonas Bergman
 */


#include "OsmAccess.h"  // class implemented

// AccessRule ----------------------------------------------------------------
OsmAccess::AccessRule::AccessRule(
    std::initializer_list<OsmAccess::AccessType> allowedTypes)
    : allowAccessToTypes(allowedTypes)
{
}

bool
OsmAccess::AccessRule::hasAccess(OsmAccess::AccessType type) const
{
    auto it = std::find(allowAccessToTypes.begin(),
        allowAccessToTypes.end(),
        type);
    return it != allowAccessToTypes.end();
}


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
OsmAccess::OsmAccess(OsmAccess::AccessType type)
    : mType(type)
{}

//============================= OPERATORS ====================================

//static
OsmAccess::AccessType
OsmAccess::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < NR_ACCESS_TYPES; ++i)
    {
        if(rTypeString == OsmAccess::sTypeStrings[i])
        {
            return static_cast<AccessType>(i);
        }
    }
    throw OsmException("OsmAccess:parseString: Unknown Access Type string.");
}

//static
std::string
OsmAccess::toString(OsmAccess::AccessType accessType)
{
    if(accessType >= NR_ACCESS_TYPES)
    {
        throw OsmException("OsmAccess:toString: Unknown Access Type");
    }
    return OsmAccess::sTypeStrings[accessType];
}

std::string
OsmAccess::toString() const
{
    return sTypeStrings[this->mType];
}

bool
OsmAccess::allowsAccess(OsmAccess::AccessRule rule) const
{
    return rule.hasAccess(mType);
}

OsmAccess::AccessType
OsmAccess::accessType() const
{
    return mType;
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const std::string OsmAccess::sTypeStrings[] =
{
    "yes",
    "private",
    "no",
    "permissive",
    "agricultural",
    "use_sidepath",
    "delivery",
    "designated",
    "dismount",
    "discouraged",
    "forestry",
    "destination",
    "customers"
};
