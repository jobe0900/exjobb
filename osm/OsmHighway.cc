/*
 * OsmHighway.cc
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */


#include "OsmHighway.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================

//============================= OPERATORS ====================================

//static
OsmHighway::HighwayType
OsmHighway::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < NR_HIGHWAY_TYPES; ++i)
    {
        if(rTypeString == OsmHighway::typeStrings[i])
        {
            return static_cast<HighwayType>(i);
        }
    }
    throw OsmException("OsmHighway:parseString: Unknown Highway Type string.");
}

//static
std::string
OsmHighway::toString(OsmHighway::HighwayType highwayType)
{
    if(highwayType >= NR_HIGHWAY_TYPES)
    {
        throw OsmException("OsmHighway:toString: Unknown Highway Type");
    }
    return OsmHighway::typeStrings[highwayType];
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const std::string OsmHighway::typeStrings[] =
{
    "motorway",
    "trunk",
    "primary",
    "secondary",
    "tertiary",
    "unclassified",
    "residential",
    "service",

    "motorway_link",
    "trunk_link",
    "primary_link",
    "secondary_link",
    "tertiary_link",

    "living_street",
    "bus_guideway",
    "road"
};
