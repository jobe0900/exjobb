/*
 * OsmVehicle.cc
 *
 * @author  Jonas Bergman
 */

#include "OsmVehicle.h"  // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
OsmVehicle::OsmVehicle(OsmVehicle::VehicleType type)
    : mType(type)
{}

//============================= OPERATORS ====================================

//static
OsmVehicle::VehicleType
OsmVehicle::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < NR_VEHICLE_TYPES; ++i)
    {
        if(rTypeString == OsmVehicle::sTypeStrings[i])
        {
            return static_cast<VehicleType>(i);
        }
    }
    throw OsmException("OsmVehicle:parseString: Unknown Vehicle Type string.");
}

//static
std::string
OsmVehicle::toString(OsmVehicle::VehicleType vehicleType)
{
    if(vehicleType >= NR_VEHICLE_TYPES)
    {
        throw OsmException("OsmVehicle:toString: Unknown Vehicle Type");
    }
    return OsmVehicle::sTypeStrings[vehicleType];
}

std::string
OsmVehicle::toString() const
{
    return sTypeStrings[this->mType];
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
const std::string OsmVehicle::sTypeStrings[] =
{
    "motorcar",
    "goods",
    "hgv",
    "psv",
    "lhv",
    "motor_vehicle",
    "vehicle"
};
