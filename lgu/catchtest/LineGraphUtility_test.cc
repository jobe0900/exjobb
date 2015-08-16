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
		    std::string config_file("catchtest/testsettings/mikh_restr_0617-testsettings.json");

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
                LineGraphType* p_lg = lgu.getLineGraph();

                THEN ("we should have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                THEN ("we should be able to print info for lines")
                {
                    int i = 0;
                    for(auto it = boost::edges(*p_lg);
                        (i < 10) && (it.first != it.second) ;
                        ++it.first, ++i)
                    {
                        const auto& line = *(it.first);
                        NodeIdType lg_source_id =
                            boost::get(&LineGraphLine::lgSourceNodeId, *p_lg, line);
                        NodeIdType lg_target_id =
                            boost::get(&LineGraphLine::lgTargetNodeId, *p_lg, line);
                        VertexIdType topo_via_vertex_id =
                            boost::get(&LineGraphLine::topoViaVertexId, *p_lg, line);
                        double cost =
                            boost::get(&LineGraphLine::cost, *p_lg, line);

                        INFO ("LINE:   lg_source_id: " << lg_source_id
                            << ", lg_target_id: " << lg_target_id
                            << ", topo_via_vertex_id: " << topo_via_vertex_id
                            << ", cost: " << cost << "\n");
                        REQUIRE (true);

                    }
                }

                delete p_lg;
            }

            WHEN ("asking to update restrictions and costs")
            {
                lgu.updateRestrictionsAndCosts();
                LineGraphType* p_lg {nullptr};
                p_lg = lgu.getLineGraph();

                THEN ("we should still be able to have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                delete p_lg;
            }

            WHEN ("asking to update topology")
            {
                lgu.updateTopology();
                LineGraphType* p_lg {nullptr};
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

