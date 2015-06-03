/*
 * Configuration.cc
 *
 *  Created on: 14 maj 2015
 *      Author: jonas
 */

#include "Configuration.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//Configuration::Configuration()
//    : mDbConfig(), mVehicleConfig()
//{}

//Configuration::~Configuration()
//{
//    delete mpDbConfig;
//    delete mpVehicleConfig;
//}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
const DatabaseConfig&
Configuration::getDatabaseConfig() const
{
    return mDbConfig;
}

const TopologyConfig&
Configuration::getTopologyConfig() const
{
    return mTopoConfig;
}

const VehicleConfig&
Configuration::getVehicleConfig() const
{
    return mVehicleConfig;
}

//void
//Configuration::setDatabaseConfig(const DatabaseConfig* pDbConfig)
//{
//    mpDbConfig = pDbConfig;
//}
//
//void
//Configuration::setVehicleConfig(const VehicleConfig* pVehicleConfig)
//{
//    mpVehicleConfig = pVehicleConfig;
//}
//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


