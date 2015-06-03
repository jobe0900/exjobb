/*
 * Configuration.cc
 *
 *  Created on: 2015-05-14
 *      Author: Jonas Bergman
 */

#include "Configuration.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
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

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


