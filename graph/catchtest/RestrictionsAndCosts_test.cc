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

// TURN RESTRICTION //////////////////////////////////////////////////////////

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

// BARRIER BLOCK //////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with barrier block",
    "[graph][restrictions][block][mikhailovsk]")
{
    // block on node 1706164751 on way 158421713
    try
    {
        std::string orig_config_file("catchtest/testsettings/"
            "restrictions/mikhailovsk-original.json");
        ConfigurationReader orig_config_reader(orig_config_file);
        Configuration orig_config;
        orig_config_reader.fillConfiguration(orig_config);
        PostGisProvider orig_pgp(orig_config);
        Topology orig_topology;
        orig_pgp.getTopology(orig_topology);
        orig_pgp.setRestrictionsAndCosts(orig_topology);
        Graph orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration to build a Graph with barrier block ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-barrier_block.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            Graph graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier block")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be 2 more edges "
                      "in original than in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges() - 2) == graph.nrEdges());
                }

                THEN ("there should be 2 more nodes "
                      "in original than in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes() - 2) == graph.nrNodes());
                }

                THEN ("there should be 10 more lines "
                      "in original than in restricted")
                {
                    // 4 bidirectional edges connecting = 8 lines
                    // 2 u-turns = 2 lines
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines() - 10) == graph.nrLines());
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

SCENARIO ("Building graph of Partille with barrier block",
    "[graph][restrictions][block][partille]")
{
    // block on node 249292683 on way 28050664
    try
    {
        std::string orig_config_file("catchtest/testsettings/"
            "restrictions/partille-original.json");
        ConfigurationReader orig_config_reader(orig_config_file);
        Configuration orig_config;
        orig_config_reader.fillConfiguration(orig_config);
        PostGisProvider orig_pgp(orig_config);
        Topology orig_topology;
        orig_pgp.getTopology(orig_topology);
        orig_pgp.setRestrictionsAndCosts(orig_topology);
        Graph orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration to build a Graph with barrier block ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-barrier_block.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            Graph graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier block")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be 2 more edges "
                      "in original than in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges() - 2) == graph.nrEdges());
                }

                THEN ("there should be 2 more nodes "
                      "in original than in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes() - 2) == graph.nrNodes());
                }

                THEN ("there should be 8 more lines "
                      "in original than in restricted")
                {
                    // 3 bidirectional edges connecting = 6 lines
                    // (1 cycleway = 0 lines)
                    // 2 u-turns = 2 lines
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines() - 8) == graph.nrLines());
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

//SCENARIO ("Building graph of Partille with barrier block",
//    "[graph][restrictions][block][partille]")
//{
//    try
//    {
//        std::string orig_config_file("catchtest/testsettings/"
//            "restrictions/mikhailovsk-original.json")
//        ConfigurationReader orig_config_reader(orig_config_file);
//        Configuration orig_config;
//        orig_config_reader.fillConfiguration(orig_config);
//        PostGisProvider orig_pgp(orig_config);
//        Topology orig_topology;
//        orig_pgp.getTopology(orig_topology);
//        orig_pgp.setRestrictionsAndCosts(orig_topology);
//        Graph orig_graph(orig_topology, orig_config);
//
//        // ===================================================================
//        GIVEN ("Configuration to build a Graph with turn restrictions ")
//        {
//            std::string config_file("catchtest/testsettings/"
//                "restrictions/partille-barrier_block.json");
//            ConfigurationReader config_reader(config_file);
//            Configuration config;
//            config_reader.fillConfiguration(config);
//
//            PostGisProvider pgp(config);
//
//            Topology topology;
//            pgp.getTopology(topology);
//            pgp.setRestrictionsAndCosts(topology);
//
//            Graph graph_restr(topology, config);
//
////            Graph graph_unrestr(topology, config, false);
//
//            // ...............................................................
//            WHEN ("Adding a turning restriction")
//            {
//
//                THEN ("there should be equally many vertices "
//                      "in restricted and unrestricted")
//                {
//                    INFO ("  Restricted # Vertices: " << graph_restr.nrVertices());
//                    INFO ("UNRestricted # Vertices: " << graph_unrestr.nrVertices());
//                    REQUIRE (graph_restr.nrVertices() == graph_unrestr.nrVertices());
//                }
//
//                THEN ("there should be equally many edges "
//                      "in restricted and unrestricted")
//                {
//                    INFO ("  Restricted # Edges:    " << graph_restr.nrEdges());
//                    INFO ("UNRestricted # Edges:    " << graph_unrestr.nrEdges());
//                    REQUIRE (graph_restr.nrEdges() == graph_unrestr.nrEdges());
//                }
//
//                THEN ("there should be equally many nodes "
//                      "in restricted and unrestricted")
//                {
//                    INFO ("  Restricted # Nodes:    " << graph_restr.nrNodes());
//                    INFO ("UNRestricted # Nodes:    " << graph_unrestr.nrNodes());
//                    REQUIRE (graph_restr.nrNodes() == graph_unrestr.nrNodes());
//                }
//
//                THEN ("there should be 1 lines less "
//                      "in restricted than unrestricted")
//                {
//                    // one connection less
//                    INFO ("  Restricted # Lines:    " << graph_restr.nrLines());
//                    INFO ("UNRestricted # Lines:    " << graph_unrestr.nrLines());
//                    REQUIRE (graph_restr.nrLines() == graph_unrestr.nrLines() - 1);
//                }
//            }
//        }
//    }
//    catch (ConfigurationException& e)
//    {
//        INFO(e.what());
//        REQUIRE (false);    // force output of error and failure
//    }
//    catch (MapProviderException& dbe)
//    {
//        INFO(dbe.what());
//        REQUIRE (false);    // force output of error and failure
//
//    }
//}
