/*
 * Restrictions_test.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"

#include "../Restrictions.h"

SCENARIO ("Adding and fetching vehicle restrictions", "[restrictions][vehicle]")
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

            r.setVehiclePropertyRestrictionForEdge(id, vp);

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

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the type should be in the vector of restriction types for edge")
            {
                std::vector<Restrictions::RestrictionType> rt = r.restrictionTypes(id);
                auto it = std::find(
                    rt.begin(),
                    rt.end(),
                    Restrictions::VEHICLE_PROPERTIES);
                REQUIRE (it != rt.end());
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the type should be reported as a restriction type for edge")
            {
                REQUIRE (r.hasRestriction(id, Restrictions::VEHICLE_PROPERTIES));
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

        //....................................................................
        WHEN ("we query for restriction for unknown edge")
        {
            THEN ("we should get a false")
            {
                REQUIRE_FALSE (
                    r.hasRestriction(567, Restrictions::VEHICLE_PROPERTIES));
            }
        }

        //....................................................................
        WHEN ("we query for restriction types for unknown edge")
        {
            std::vector<Restrictions::RestrictionType> rt = r.restrictionTypes(567);
            THEN ("we should get an empty vector")
            {
                REQUIRE (rt.size() == 0);
            }
        }
    }
}

SCENARIO ("Adding and fetching general access restrictions", "[restrictions][general]")
{
    // -----------------------------------------------------------------------
    GIVEN ("a Restrictions object")
    {
        Restrictions r;
        //....................................................................
        WHEN ("we try to add access restriction DELIVERY to edge id 4")
        {
            OsmAccess deliveryAccess({OsmAccess::DELIVERY});
            EdgeIdType id = 4;

            r.setGeneralAccessRestrictionForEdge(id, deliveryAccess);

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("we should get be able to get the property and check")
            {
                REQUIRE (r.generalAccess(id).accessType() == OsmAccess::DELIVERY);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the type should be in the vector of restriction types for edge")
            {
                std::vector<Restrictions::RestrictionType> rt = r.restrictionTypes(id);
                auto it = std::find(
                    rt.begin(),
                    rt.end(),
                    Restrictions::GENERAL_ACCESS);
                REQUIRE (it != rt.end());
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the type should be reported as a restriction type for edge")
            {
                REQUIRE (r.hasRestriction(id, Restrictions::GENERAL_ACCESS));
            }

        }

        //....................................................................
        WHEN ("we try to fetch vehicle property restriction for unknown edge")
        {
            THEN ("we should get a RestrictionsException")
            {
                REQUIRE_THROWS_AS (
                    r.generalAccess(567),
                    RestrictionsException&);
            }
        }
    }
}






