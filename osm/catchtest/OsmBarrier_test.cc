/*
 * OsmBarrier_test.cc
 *
 *  Created on: 2015-05-25
 *      Author: Jonas Bergman
 */

#include "../OsmBarrier.h"


#include "../../catchtest/catch.hpp"


SCENARIO ("OsmBarrier functionality testing", "[osm][barrier]")
{
    try
    {
        // -------------------------------------------------------------------
        GIVEN ("a valid string of an access type")
        {
            std::string type_string("swing_gate");

            //................................................................
            WHEN ("parsing string to a BarrierType")
            {
                OsmBarrier::BarrierType type =
                    OsmBarrier::parseString(type_string);
                THEN ("we should get the corresponding type")
                {
                    REQUIRE (type == OsmBarrier::BarrierType::SWING_GATE);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("an invalid string of a barrier type")
        {
            std::string type_string("foo");

            //................................................................
            WHEN ("parsing string to a BarrierType")
            {
                THEN ("we should get an OsmException")
                {
                    REQUIRE_THROWS_AS (OsmBarrier::parseString(type_string),
                        OsmException&);
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a barrier type")
        {
            OsmBarrier type(OsmBarrier::CATTLE_GRID);

            //................................................................
            WHEN ("converting type to a string")
            {
                THEN ("we should the corresponding string")
                {
                    REQUIRE (type.toString() == "cattle_grid");
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a restriction rule")
        {
            OsmBarrier::RestrictionsRule rule({OsmBarrier::YES, OsmBarrier::BOLLARD});

            //................................................................
            WHEN ("checking if access is restricted for type not in rule")
            {
                OsmBarrier type(OsmBarrier::SPIKES);

                THEN ("we should be told there is no restriction on access")
                {
                    REQUIRE_FALSE (type.restrictsAccess(rule));
                }
            }
            //................................................................
            WHEN ("checking if access is restricted for type in rule")
            {
                OsmBarrier type(OsmBarrier::BOLLARD);

                THEN ("we should be told there restriction on access")
                {
                    REQUIRE (type.restrictsAccess(rule));
                }
            }
        }

        // -------------------------------------------------------------------
        GIVEN ("a cost rule")
        {
            OsmBarrier::CostsRule rule({OsmBarrier::GATE, OsmBarrier::DEBRIS});

            //................................................................
            WHEN ("checking if access costs for type not in rule")
            {
                OsmBarrier type(OsmBarrier::YES);

                THEN ("we should be told there is no cost on access")
                {
                    REQUIRE_FALSE (type.costsToPass(rule));
                }
            }
            //................................................................
            WHEN ("checking if access costs for type in rule")
            {
                OsmBarrier type(OsmBarrier::DEBRIS);

                THEN ("we should be told there is cost on access")
                {
                    REQUIRE (type.costsToPass(rule));
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




