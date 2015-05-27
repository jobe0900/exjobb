/*
 * Restrictions_test.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"

#include "../Restrictions.h"

SCENARIO ("Adding and fetching vehicle restrictions",
            "[restrictions][vehicleprop]")
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

SCENARIO ("Adding and fetching vehicle type restrictions",
          "[restrictions][vehicletype]")
{
    //------------------------------------------------------------------------
    GIVEN ("A restrictions object, an edgeId and vehicle types")
    {
        Restrictions r;
        EdgeIdType   e1 {1};
        EdgeIdType   e2 {2};
        OsmVehicle::VehicleType v1 {OsmVehicle::MOTORCAR};
        OsmVehicle::VehicleType v2 {OsmVehicle::PSV};
        OsmAccess    rest_destination {OsmAccess::DESTINATION};
        OsmAccess    rest_no {OsmAccess::NO};

        //....................................................................
        WHEN ("adding restriction")
        {
            r.setVehicleTypeAccessRestrictionsForEdge(e1, v1, rest_destination);

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should be reported as installed for edge")
            {
                REQUIRE (
                    r.hasRestriction(e1, Restrictions::VEHICLE_TYPE_ACCESS));
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should NOT be reported as installed for another edge")
            {
                REQUIRE_FALSE (
                    r.hasRestriction(e2, Restrictions::VEHICLE_TYPE_ACCESS));
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should have only one restriction")
            {
                auto type_vector = r.vehicleTypesWithRestrictions(e1);
                REQUIRE (type_vector.size() == 1);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("no other vehicle type should be reported as installed")
            {
                REQUIRE_FALSE (r.hasVehicleTypeAccessRestriction(e1, v2));
            }
        }

        //....................................................................
        WHEN ("adding two restrictions")
        {
            r.setVehicleTypeAccessRestrictionsForEdge(e1, v1, rest_destination);
            r.setVehicleTypeAccessRestrictionsForEdge(e1, v2, rest_no);

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should be reported as installed for edge")
            {
                REQUIRE (
                    r.hasRestriction(e1, Restrictions::VEHICLE_TYPE_ACCESS));
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should NOT be reported as installed for another edge")
            {
                REQUIRE_FALSE (
                    r.hasRestriction(e2, Restrictions::VEHICLE_TYPE_ACCESS));
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should have two restriction")
            {
                auto type_vector = r.vehicleTypesWithRestrictions(e1);
                REQUIRE (type_vector.size() == 2);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("we can print out which vehicle types have restrictions")
            {
                const auto& v_type_vector = r.vehicleTypesWithRestrictions(e1);
                for(const auto& t : v_type_vector)
                {
                    INFO ("Edge " << e1 << " restriction for vehicle: "
                           << OsmVehicle::toString(t) << ": "
                           << r.vehicleTypeAccess(e1, t).toString());
                    REQUIRE (true);
                }
            }
        }

        //....................................................................
        WHEN ("adding restriction to already existing")
        {
            r.setVehicleTypeAccessRestrictionsForEdge(e1, v1, rest_destination);
            r.setVehicleTypeAccessRestrictionsForEdge(e1, v1, rest_no);

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the first should NOT be reported as installed for edge")
            {
                REQUIRE_FALSE (r.vehicleTypeAccess(e1, v1).accessType() ==
                    OsmAccess::DESTINATION);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the second should be reported as installed for edge")
            {
                REQUIRE (r.vehicleTypeAccess(e1, v1).accessType() ==
                    OsmAccess::NO);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("it should have one restriction")
            {
                auto type_vector = r.vehicleTypesWithRestrictions(e1);
                REQUIRE (type_vector.size() == 1);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("we can print out which vehicle types have restrictions")
            {
                const auto& v_type_vector = r.vehicleTypesWithRestrictions(e1);
                for(const auto& t : v_type_vector)
                {
                    INFO ("Edge " << e1 << " restriction for vehicle: "
                           << OsmVehicle::toString(t) << ": "
                           << r.vehicleTypeAccess(e1, t).toString());
                    REQUIRE (true);
                }
            }
        }
    }
}






