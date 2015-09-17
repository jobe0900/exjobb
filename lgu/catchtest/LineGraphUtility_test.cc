/*
 * LineGraphUtility_test.cc
 *
 *  Created on: 2015-05-18
 *      Author: Jonas Bergman
 */

#include "../LineGraphUtility.h"
#include <chrono>
using namespace std::chrono;


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

                THEN ("we can try to persist line graph")
                {
                    try
                    {
                        lgu.persistLineGraph();
                        INFO ("Persisted line graph");
                        REQUIRE (true);
                    }
                    catch (LineGraphUtilityException& lgue)
                    {
                        INFO (lgue.what());
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


SCENARIO ("LineGraphUtility timing", "[lgu][timing]")
{
    try
    {
        GIVEN ("a valid config file for Mikhailovsk without temporary topology")
        {
//		    std::string config_file("catchtest/testsettings/part_0815_settings.json");
		    std::string config_file("catchtest/testsettings/mikh_restr_0617-testsettings.json");
//		    std::string config_file("catchtest/testsettings/part_0815_settings.json");

            WHEN ("when timing request for a LineGraph")
            {
                // start timing
                std::chrono::high_resolution_clock::time_point t1 =
                    std::chrono::high_resolution_clock::now();

                LineGraphUtility lgu(config_file);
                LineGraphType* p_lg = lgu.getLineGraph();

                // end timing;
                std::chrono::high_resolution_clock::time_point t2 =
                    std::chrono::high_resolution_clock::now();

                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

                THEN ("we should have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                THEN ("we should have a timing")
                {
                    INFO ("Duration for Mikhailovsk without temporary topology "
                        << duration << " microseconds");
                    REQUIRE (true);
                }

                delete p_lg;
            }
        }

        GIVEN ("a valid config file for Partille without temporary topology")
        {
		    std::string config_file("catchtest/testsettings/part_0815_settings.json");

            WHEN ("when timing request for a LineGraph")
            {
                // start timing
                std::chrono::high_resolution_clock::time_point t1 =
                    std::chrono::high_resolution_clock::now();

                LineGraphUtility lgu(config_file);
                LineGraphType* p_lg = lgu.getLineGraph();

                // end timing;
                std::chrono::high_resolution_clock::time_point t2 =
                    std::chrono::high_resolution_clock::now();

                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

                THEN ("we should have a line graph")
                {
                    REQUIRE (p_lg != nullptr);
                    REQUIRE (boost::num_edges(*p_lg) > 0);
                }

                THEN ("we should have a timing")
                {
                    INFO ("Duration for Partille without temporary topology "
                        << duration << " microseconds");
                    REQUIRE (true);
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
