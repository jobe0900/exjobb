/*
 * EpochMsTimeToString_test.cc
 *
 *  Created on: 8 maj 2015
 *      Author: jonas
 */

#include <string>
#include "../../catchtest/catch.hpp"
#include "../EpochMsTimeToString.h"


SCENARIO ("Wanting to get the timestring") {
	GIVEN ("the current time") {
		WHEN ("asking for string representation") {
			TimeToStringMaker* p_time_to_str = new EpochMsTimeToString();
			std::string timeMs = p_time_to_str->getCurrentTimeString();
			delete p_time_to_str;
			THEN ("we should get a string") {
				INFO ("current time is " << timeMs );
				REQUIRE (timeMs.length() > 0);
			}
		}
	}
}


