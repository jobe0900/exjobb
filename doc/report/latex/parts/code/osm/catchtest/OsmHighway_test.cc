/*
 * OsmHighway_test.cc
 *
 * @author  Jonas Bergman
 */

#include "../OsmHighway.h"
#include "../../catchtest/catch.hpp"

SCENARIO ("OsmHighway functionality testing", "[osm][highway]")
{
    try
    {
        // -------------------------------------------------------------------
        GIVEN ("a valid string of a highway type")
        {
            std::string type_string("primary");

            //................................................................
            WHEN ("parsing string to a HighwayType")
            {
                OsmHighway::HighwayType type =
                    OsmHighway::parseString(type_string);
                THEN ("we should get the corresponding type")
                {
                    REQUIRE (type == OsmHighway::HighwayType::PRIMARY);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an invalid string of a highway type")
        {
            std::string type_string("foo");

            //................................................................
            WHEN ("parsing string to a HighwayType")
            {
                THEN ("we should get an OsmException")
                {
                    REQUIRE_THROWS_AS (OsmHighway::parseString(type_string),
                    OsmException&);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a highway type")
        {
            OsmHighway type(OsmHighway::PRIMARY);

            //................................................................
            WHEN ("converting type to a string")
            {
                THEN ("we should the corresponding strng")
                {
                    REQUIRE (type.toString() == "primary");
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
