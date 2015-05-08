/*
 * EpochMsTimeToString.cc
 *
 *  Created on: 2015-05-08
 *      Author: Jonas Bergman
 */

#include "EpochMsTimeToString.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
EpochMsTimeToString::EpochMsTimeToString()
{}

EpochMsTimeToString::~EpochMsTimeToString()
{}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
//static
std::string
EpochMsTimeToString::getCurrentTimeString()
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



