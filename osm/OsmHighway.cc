/*
 * OsmHighway.cc
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */


#include "OsmHighway.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
OsmHighway::OsmHighway(OsmHighway::HighwayType type)
    : mType(type)
{}

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
OsmHighway::HighwayType
OsmHighway::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < NR_HIGHWAY_TYPES; ++i)
    {
        if(rTypeString == OsmHighway::sTypeStrings[i])
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
    return OsmHighway::sTypeStrings[highwayType];
}

std::string
OsmHighway::toString() const
{
    return sTypeStrings[this->mType];
}

//============================= ACESS      ===================================
//static
const std::vector<std::string>&
OsmHighway::typeStrings()
{
    return OsmHighway::sTypeStrings;
}

// static
std::string
OsmHighway::typesAsCommaSeparatedString()
{
    std::string cols;
    std::stringstream ss;
    ss << "(";
    for(size_t i = 0; i < sTypeStrings.size(); ++i)
    {
        ss << "'" << sTypeStrings[i] << "'";
        if(i < sTypeStrings.size() - 1)
        {
            ss << ", ";
        }
    }
    ss << ")";
    return ss.str();
}
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const std::vector<std::string> OsmHighway::sTypeStrings
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
