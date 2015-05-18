/*
 * LineGraphUtility_test.cc
 *
 *  Created on: 2015-05-18
 *      Author: Jonas Bergman
 */

#include "../LineGraphUtility.h"


#include "../../catchtest/catch.hpp"


SCENARIO ("LineGraphUtility construction", "[lgu][construction]")
{
    try
    {
        GIVEN ("a valid config file set up to use jsontest as provider")
        {
            std::string config_file("lgu/catchtest/jsontest-settings.json");

            //................................................................
            WHEN ("creating a LineGraphUtility")
            {
                THEN ("we should not receive an exception")
                {
                    REQUIRE_NOTHROW(LineGraphUtility lgu(config_file));
                }
            }
        }
    }
    catch (LineGraphUtilityException& lgue)
    {
        INFO(lgue.what());
        REQUIRE (false);    // force output of error and failure
    }
    catch (const std::exception& e)
    {
        INFO(e.what());
        REQUIRE (false);    // force output of error and failure

    }
}


