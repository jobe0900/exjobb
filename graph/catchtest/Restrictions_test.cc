/*
 * Restrictions_test.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"

#include "../Restrictions.h"

SCENARIO ("Adding and fetching restrictions", "[restrictions]")
{
    // -----------------------------------------------------------------------
    GIVEN ("a Restrictions object")
    {
        Restrictions r;
        //....................................................................
        WHEN ("we try to add vehicle property restriction with max speed 60 "
              "to edge id 4")
        {
            int max_speed = 60;
            EdgeIdType id = 4;
            Restrictions::VehicleProperties vp;
            vp.maxSpeed = max_speed;

            r.setVehiclePropertyRestricionForEdge(id, vp);

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("we should get be able to get the property and check")
            {
                REQUIRE (r.vehicleProperties(id).maxSpeed == max_speed);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("we should get be able to get reference to property and change")
            {
                Restrictions::VehicleProperties& r_vp = r.vehicleProperties(id);
                r_vp.maxHeight = 10.0;
                REQUIRE (r.vehicleProperties(id).maxHeight == Approx(10.0));
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should be reported as having restriction for edge")
            {
                REQUIRE (r.hasVehiclePropertyRestriction(id));
            }
        }

        //....................................................................
        WHEN ("we try to fetch vehicle property restriction for unknown edge")
        {
            THEN ("we should get a RestrictionsException")
            {
                REQUIRE_THROWS_AS (
                    r.vehicleProperties(567),
                    RestrictionsException&);
            }
        }

        //....................................................................
        WHEN ("we query for vehicle property restriction for unknown edge")
        {
            THEN ("we should get a false")
            {
                REQUIRE_FALSE (r.hasVehiclePropertyRestriction(567));
            }
        }
    }
}






