/* Tests for th
 * Graph_test.cc
 *e different kind of restrictions
 *      Author: Jonas Bergman
 */

#include <iostream>

#include "../../catchtest/catch.hpp"

#include "../Graph.h"
#include "../Topology.h"
#include "../../config/ConfigurationReader.h"
#include "../../mapprovider/postgis/PostGisProvider.h"

SCENARIO ("Building graph of Mikhailovsk with turn restrictions",
    "[graph][restrictions][turn][mikhailovsk]")
{
    try
    {

        // ===================================================================
        GIVEN ("Configuration to build a Graph with turn restrictions ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-turn_no_right.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            Graph graph_restr(topology, config);

            Graph graph_unrestr(topology, config, false);

            // ...............................................................
            WHEN ("Adding a turning restriction")
            {

                THEN ("there should be equally many vertices "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Vertices: " << graph_restr.nrVertices());
                    INFO ("UNRestricted # Vertices: " << graph_unrestr.nrVertices());
                    REQUIRE (graph_restr.nrVertices() == graph_unrestr.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Edges:    " << graph_restr.nrEdges());
                    INFO ("UNRestricted # Edges:    " << graph_unrestr.nrEdges());
                    REQUIRE (graph_restr.nrEdges() == graph_unrestr.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Nodes:    " << graph_restr.nrNodes());
                    INFO ("UNRestricted # Nodes:    " << graph_unrestr.nrNodes());
                    REQUIRE (graph_restr.nrNodes() == graph_unrestr.nrNodes());
                }

                THEN ("there should be 1 lines less "
                      "in restricted than unrestricted")
                {
                    // one connection less
                    INFO ("  Restricted # Lines:    " << graph_restr.nrLines());
                    INFO ("UNRestricted # Lines:    " << graph_unrestr.nrLines());
                    REQUIRE (graph_restr.nrLines() == graph_unrestr.nrLines() - 1);
                }
            }
        }
    }
    catch (ConfigurationException& e)
    {
        INFO(e.what());
        REQUIRE (false);    // force output of error and failure
    }
    catch (MapProviderException& dbe)
    {
        INFO(dbe.what());
        REQUIRE (false);    // force output of error and failure

    }
}

SCENARIO ("Building graph of Partille with turn restrictions",
    "[graph][restrictions][turn][partille]")
{
    try
    {

        // ===================================================================
        GIVEN ("Configuration to build a Graph with turn restrictions ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-turn_no_left.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            Graph graph_restr(topology, config);

            Graph graph_unrestr(topology, config, false);

            // ...............................................................
            WHEN ("Adding a turning restriction")
            {

                THEN ("there should be equally many vertices "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Vertices: " << graph_restr.nrVertices());
                    INFO ("UNRestricted # Vertices: " << graph_unrestr.nrVertices());
                    REQUIRE (graph_restr.nrVertices() == graph_unrestr.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Edges:    " << graph_restr.nrEdges());
                    INFO ("UNRestricted # Edges:    " << graph_unrestr.nrEdges());
                    REQUIRE (graph_restr.nrEdges() == graph_unrestr.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Nodes:    " << graph_restr.nrNodes());
                    INFO ("UNRestricted # Nodes:    " << graph_unrestr.nrNodes());
                    REQUIRE (graph_restr.nrNodes() == graph_unrestr.nrNodes());
                }

                THEN ("there should be 1 lines less "
                      "in restricted than unrestricted")
                {
                    // one connection less
                    INFO ("  Restricted # Lines:    " << graph_restr.nrLines());
                    INFO ("UNRestricted # Lines:    " << graph_unrestr.nrLines());
                    REQUIRE (graph_restr.nrLines() == graph_unrestr.nrLines() - 1);
                }
            }
        }
    }
    catch (ConfigurationException& e)
    {
        INFO(e.what());
        REQUIRE (false);    // force output of error and failure
    }
    catch (MapProviderException& dbe)
    {
        INFO(dbe.what());
        REQUIRE (false);    // force output of error and failure

    }
}
