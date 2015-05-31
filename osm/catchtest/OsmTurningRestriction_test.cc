/*
 * OsmTurningRestriction_test.cc
 *
 *  Created on: 2015-05-26
 *      Author: Jonas Bergman
 */

#include "../OsmTurningRestriction.h"


#include "../../catchtest/catch.hpp"


SCENARIO ("OsmTurningRestriction functionality testing", "[osm][turning]")
{
    try
    {
        // -------------------------------------------------------------------
        GIVEN ("a valid string of a turning restriction type")
        {
            std::string type_string("no_straight_on");

            //................................................................
            WHEN ("parsing string to an TurningRestrictionType")
            {
                OsmTurningRestriction::TurningRestrictionType type =
                    OsmTurningRestriction::parseString(type_string);
                THEN ("we should get the corresponding type")
                {
                    REQUIRE ( type ==
                        OsmTurningRestriction::
                            TurningRestrictionType::NO_STRAIGHT_ON);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an invalid string of an access type")
        {
            std::string type_string("foo");

            //................................................................
            WHEN ("parsing string to a TurningRestrictionType")
            {
                THEN ("we should get an OsmException")
                {
                    REQUIRE_THROWS_AS (
                        OsmTurningRestriction::parseString(type_string),
                        OsmException&);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a turning restriction from edge 1 via vertex 2 to edge 3")
        {
            OsmTurningRestriction restr(OsmTurningRestriction::NO_LEFT_TURN,
                1,
                2,
                3);

            //................................................................
            WHEN ("converting restriction to a string")
            {
                THEN ("we should have a nice string to print")
                {
                    INFO (restr.toString());
                    REQUIRE (true);
                }
            }

            //................................................................
            WHEN ("checking the restriction type as a string")
            {
                THEN ("the strings should match")
                {
                    REQUIRE (
                        OsmTurningRestriction::toString(
                            OsmTurningRestriction::NO_LEFT_TURN)
                        == restr.typeToString());
                }
            }

            //................................................................
            WHEN ("checking if edge id 4 is part of restriction")
            {
                THEN ("we should receive negative answer")
                {
                    REQUIRE_FALSE (restr.isInRestriction(4));
                 }
            }

            //................................................................
            WHEN ("checking if edge id 3 is part of restriction")
            {
                THEN ("we should receive positive answer")
                {
                    REQUIRE (restr.isInRestriction(3));
                }
            }

        }

        // -------------------------------------------------------------------
        GIVEN ("a turning restriction from edge 1 via osm edges 20 and 30 to edge 4")
        {
            OsmTurningRestriction restr(OsmTurningRestriction::NO_RIGHT_TURN,
                1,
                "w20,w30",
                4);

            //................................................................
            WHEN ("checking if edge id 5 is part of restriction")
            {
                THEN ("we should receive negative answer")
                {
                    REQUIRE_FALSE (restr.isInRestriction(5));
                 }
            }

            //................................................................
            WHEN ("checking if edge id 30 is part of restriction")
            {
                THEN ("we should receive negative answer")
                {
                    REQUIRE_FALSE (restr.isInRestriction(30));
                }
            }

            //................................................................
            WHEN ("converting restriction to a string")
            {
                THEN ("we should have a nice string to print")
                {
                    INFO (restr.toString());
                    REQUIRE (true);
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
