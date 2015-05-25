/*
 * OsmAccess_test.cc
 *
 *  Created on: 2015-05-24
 *      Author: Jonas Bergman
 */

#include "../OsmAccess.h"


#include "../../catchtest/catch.hpp"


SCENARIO ("OsmAccess functionality testing", "[osm][access]")
{
    try
    {
        // -------------------------------------------------------------------
        GIVEN ("a valid string of an access type")
        {
            std::string type_string("designated");

            //................................................................
            WHEN ("parsing string to an AccessType")
            {
                OsmAccess::AccessType type =
                    OsmAccess::parseString(type_string);
                THEN ("we should get the corresponding type")
                {
                    REQUIRE (type == OsmAccess::AccessType::DESIGNATED);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an invalid string of an access type")
        {
            std::string type_string("foo");

            //................................................................
            WHEN ("parsing string to a AccessType")
            {
                THEN ("we should get an OsmException")
                {
                    REQUIRE_THROWS_AS (OsmAccess::parseString(type_string),
                        OsmException&);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an access type")
        {
            OsmAccess type(OsmAccess::DELIVERY);

            //................................................................
            WHEN ("converting type to a string")
            {
                THEN ("we should the corresponding string")
                {
                    REQUIRE (type.toString() == "delivery");
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an access rule")
        {
            OsmAccess::AccessRule rule({OsmAccess::YES, OsmAccess::PERMISSIVE});

            //................................................................
            WHEN ("checking for access for type not in rule")
            {
                OsmAccess type(OsmAccess::DELIVERY);

                THEN ("we should not be allowed access")
                {
                    REQUIRE_FALSE (type.hasAccess(rule));
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




