/*
 * OsmVehicle_test.cc
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */

#include "../OsmVehicle.h"


#include "../../catchtest/catch.hpp"


SCENARIO ("OsmVehicle functionality testing", "[osm][vehicle]")
{
    try
    {
        // -------------------------------------------------------------------
        GIVEN ("a valid string of a vehicle type")
        {
            std::string type_string("motorcar");

            //................................................................
            WHEN ("parsing string to a VehicleType")
            {
                OsmVehicle::VehicleType type =
                    OsmVehicle::parseString(type_string);
                THEN ("we should get the corresponding type")
                {
                    REQUIRE (type == OsmVehicle::VehicleType::MOTORCAR);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an invalid string of a vehicle type")
        {
            std::string type_string("foo");

            //................................................................
            WHEN ("parsing string to a VehicleType")
            {
                THEN ("we should get an OsmException")
                {
                    REQUIRE_THROWS_AS (OsmVehicle::parseString(type_string),
                    OsmException&);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a Vehicle type")
        {
            OsmVehicle type(OsmVehicle::PSV);

            //................................................................
            WHEN ("converting type to a string")
            {
                THEN ("we should the corresponding string")
                {
                    REQUIRE (type.toString() == "psv");
                }
            }
        }
    }
    catch (OsmException& oe)
    {
        INFO(oe.what());
        REQUIRE (false);    // force output of error and failure
    }
    catch (const std::exception& e)
    {
        INFO(e.what());
        REQUIRE (false);    // force output of error and failure

    }
}



