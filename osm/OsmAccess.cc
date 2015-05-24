/*
 * OsmAccess.cc
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */


#include "OsmAccess.h"  // class implemented


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



