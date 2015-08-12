/* Tests for EdgeCost class
 * EdgeCost_test.cc
 *
 *      Author: Jonas Bergman
 */


#include "../../catchtest/catch.hpp"

#include "../EdgeCost.h"

SCENARIO ("Keeping track of costs for an Edge", "[edgecost]")
{
    EdgeCost costs;

    GIVEN ("an EdgeCost object")
    {
        WHEN ("no costs are added")
        {
            THEN ("there should be no costs")
            {
                REQUIRE (costs.getCost() == 0);
                REQUIRE (costs.hasCost(EdgeCost::TRAVEL_TIME) == false);
                REQUIRE (costs.getCost(EdgeCost::BARRIER) == 0);
            }
        }

        WHEN ("travel cost is added")
        {
            costs.addCost(EdgeCost::TRAVEL_TIME, 10);

            THEN ("there should be costs")
            {
                REQUIRE (costs.getCost() > 0);
                REQUIRE (costs.hasCost(EdgeCost::TRAVEL_TIME) == true);
                REQUIRE (costs.getCost(EdgeCost::TRAVEL_TIME) == Approx(10.0));
                REQUIRE (costs.getCost(EdgeCost::BARRIER) == 0);
            }
        }

        WHEN ("two travel costs are added")
        {
            costs.addCost(EdgeCost::TRAVEL_TIME, 10);
            costs.addCost(EdgeCost::TRAVEL_TIME, 20);

            THEN ("only the last should be reported")
            {
                REQUIRE (costs.hasCost(EdgeCost::TRAVEL_TIME) == true);
                REQUIRE (costs.getCost(EdgeCost::TRAVEL_TIME) == Approx(20.0));
            }
        }

        WHEN ("travel a travel and a barrier cost are added")
        {
            costs.addCost(EdgeCost::TRAVEL_TIME, 10);
            costs.addCost(EdgeCost::BARRIER, 20);

            THEN ("the costs should be added")
            {
                REQUIRE (costs.getCost() == Approx(30.0));
                REQUIRE (costs.hasCost(EdgeCost::TRAVEL_TIME) == true);
                REQUIRE (costs.hasCost(EdgeCost::BARRIER) == true);
                REQUIRE (costs.hasCost(EdgeCost::OTHER) == false);
            }
        }
    }
}


