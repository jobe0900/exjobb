/*
 * TimeToStringMaker.cc
 *
 * @author  Jonas Bergman
 */


#include "TimeToStringMaker.h"  // class implemented

#include <chrono>

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
//static
std::string
TimeToStringMaker::getEpochMsTimeString()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch());
    return std::to_string(ms.count());
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

