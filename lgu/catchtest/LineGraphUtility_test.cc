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

SCENARIO ("LineGraphUtility operation", "[lgu][operation]")
{
    try
    {
        GIVEN ("a valid config file")
        {
		    std::string config_file("catchtest/testsettings/mikh_restr_0617-testsettings.json");
		    LineGraphUtility lgu(config_file);

            WHEN ("asking for a LineGraph")
            {
                Graph::LineGraphType* p_lg {nullptr};
                p_lg = lgu.getLineGraph();

                THEN ("we should have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                delete p_lg;
            }

            WHEN ("asking to update restrictions and costs")
            {
                lgu.updateRestrictionsAndCosts();
                Graph::LineGraphType* p_lg {nullptr};
                p_lg = lgu.getLineGraph();

                THEN ("we should still be able to have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                delete p_lg;
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

