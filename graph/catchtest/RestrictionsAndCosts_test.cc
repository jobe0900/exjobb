/* Tests for the different kind of restrictions
 *
 * Graph_test.cc
 * @author  Jonas Bergman
 */

#include <iostream>

#include "../../catchtest/catch.hpp"

#include "../Topology.h"
#include "../../config/ConfigurationReader.h"
#include "../../mapprovider/postgis/PostGisProvider.h"
#include "../GraphBuilder.h"

// TURN RESTRICTION //////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with turn restriction",
    "[graph][r_and_c][turn][mikhailovsk]")
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration to build a Graph with turn restriction ")
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

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with turn restrictions")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be 1 less line "
                      "in original than in restricted")
                {
                    // 1 right turn
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines() - 1) == graph.nrLines());
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

SCENARIO ("Building graph of Partille with turn restriction",
    "[graph][r_and_c][turn][partille]")
{
    // block on node 1706164751 on way 158421713
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration to build a Graph with turn restriction ")
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

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with turn restrictions")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be 1 less line "
                      "in original than in restricted")
                {
                    // 1 right turn
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines() - 1) == graph.nrLines());
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
    "[graph][r_and_c][block][mikhailovsk]")
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that restricts barrier block ")
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

            GraphBuilder graph(topology, config);

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
    "[graph][r_and_c][block][partille]")
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that restricts barrier block ")
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

            GraphBuilder graph(topology, config);

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


// BARRIER BOLLARD ///////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with barrier bollard",
    "[graph][r_and_c][bollard][mikhailovsk]")
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that restricts barrier bollard ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-barrier_bollard.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier bollard")
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

SCENARIO ("Building graph of Partille with barrier bollard",
    "[graph][r_and_c][bollard][partille]")
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that restricts barrier bollard ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-barrier_bollard.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier bollard")
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

// BARRIER LIFT GATE /////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with barrier lift gate",
    "[graph][r_and_c][lift_gate][mikhailovsk]")
{
    // block on node 1706164751 on way 158421713 (topo edge 649)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that does not restrict barrier lift gate ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-barrier_lift_gate.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier lift gate")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 60 on edge 649")
                {
                    EdgeIdType id = 649;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (60.0 == Approx((rest_cost - orig_cost)));
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


SCENARIO ("Building graph of Partille with barrier lift gate",
    "[graph][r_and_c][lift_gate][partille]")
{
    // lift gate on node 249292683 on way 28050664 (topo edge 267)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that does not restrict barrier lift gate ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-barrier_lift_gate.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with barrier lift gate")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 60 on edge 267")
                {
                    EdgeIdType id = 267;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (60.0 == Approx((rest_cost - orig_cost)));
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

// TRAFFIC LIGHTS ////////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with traffic signals",
    "[graph][r_and_c][traffic_signals][mikhailovsk]")
{
    // additional node on way 158421713 (topo edge id 649)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for traffic signals ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-highway_traffic_signals.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with traffic lights")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 649")
                {
                    EdgeIdType id = 649;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

SCENARIO ("Building graph of Partille with traffic signals",
    "[graph][r_and_c][traffic_signals][partille]")
{
    // additional node on way 28050664 (topo edge id 267)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for traffic signals ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-highway_traffic_signals.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with traffic lights")
            {

                THEN ("there should be equally many vertices "
                      "in original and restricted")
                {
                    INFO ("  Original # Vertices: " << orig_graph.nrVertices());
                    INFO ("Restricted # Vertices: " << graph.nrVertices());
                    REQUIRE (orig_graph.nrVertices() == graph.nrVertices());
                }

                THEN ("there should be equally many edges "
                      "in original as in restricted")
                {
                    INFO ("  Original # Edges: " << orig_graph.nrEdges());
                    INFO ("Restricted # Edges: " << graph.nrEdges());
                    REQUIRE ((orig_graph.nrEdges()) == graph.nrEdges());
                }

                THEN ("there should be equally many nodes "
                      "in original as in restricted")
                {
                    INFO ("  Original # Nodes:    " << orig_graph.nrNodes());
                    INFO ("Restricted # Nodes:    " << graph.nrNodes());
                    REQUIRE ((orig_graph.nrNodes()) == graph.nrNodes());
                }

                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 267")
                {
                    EdgeIdType id = 267;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

// STOP NODE /////////////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with stop at node before crossing",
    "[graph][r_and_c][stop_node][mikhailovsk]")
{
    // additional node on way 158421713 (topo edge id 649)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for stops ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-highway_stop_node.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 649")
                {
                    EdgeIdType id = 649;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

SCENARIO ("Building graph of Partille with stop at node before crossing",
    "[graph][r_and_c][stop_node][partille]")
{
    // additional node on way 28050664 (topo edge id 267)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for traffic signals ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-highway_stop_node.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 267")
                {
                    EdgeIdType id = 267;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

// STOP ALL //////////////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with stop for all at crossing",
    "[graph][r_and_c][stop_all][mikhailovsk]")
{
    // stop at vertex 1706164758 (topo 460)
    // affecting edges with topo id 611, 649, 661
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for stops ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-highway_stop_all.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 611")
                {
                    EdgeIdType id = 611;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
                }

                THEN ("there should be an extra cost of 30 on edge 649")
                {
                    EdgeIdType id = 649;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
                }

                THEN ("there should be an extra cost of 30 on edge 661")
                {
                    EdgeIdType id = 661;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

SCENARIO ("Building graph of Partille with stop for all at crossing",
    "[graph][r_and_c][stop_all][partille]")
{
    // stop at vertex 308018343 (topo 229)
    // affecting edges with topo id 265, 266, 267
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for traffic signals ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-highway_stop_all.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 30 on edge 265")
                {
                    EdgeIdType id = 265;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
                }

                THEN ("there should be an extra cost of 30 on edge 266")
                {
                    EdgeIdType id = 266;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
                }

                THEN ("there should be an extra cost of 30 on edge 267")
                {
                    EdgeIdType id = 267;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (30.0 == Approx((rest_cost - orig_cost)));
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

// TRAFFIC CALMING ///////////////////////////////////////////////////////////

SCENARIO ("Building graph of Mikhailovsk with speed bump at node",
    "[graph][r_and_c][traffic_calming][mikhailovsk]")
{
    // additional node on way 158421713 (topo edge id 649)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for stops ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/mikhailovsk-traffic_calming_bump.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 10 on edge 649")
                {
                    EdgeIdType id = 649;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (10.0 == Approx((rest_cost - orig_cost)));
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

SCENARIO ("Building graph of Partille with speed bump at node",
    "[graph][r_and_c][traffic_calming][partille]")
{
    // additional node on way 28050664 (topo edge id 267)
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
        GraphBuilder orig_graph(orig_topology, orig_config);

        // ===================================================================
        GIVEN ("Configuration that has cost for stops ")
        {
            std::string config_file("catchtest/testsettings/"
                "restrictions/partille-traffic_calming_hump.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph(topology, config);

            // ...............................................................
            WHEN ("Comparing original to graph with stop signs")
            {
                THEN ("there should be equally many lines "
                      "in original as in restricted")
                {
                    INFO ("  Original # Lines:    " << orig_graph.nrLines());
                    INFO ("Restricted # Lines:    " << graph.nrLines());
                    REQUIRE ((orig_graph.nrLines()) == graph.nrLines());
                }

                THEN ("there should be an extra cost of 10 on edge 267")
                {
                    EdgeIdType id = 267;
                    Cost orig_cost = orig_topology.getEdge(id).cost();
                    Cost rest_cost = topology.getEdge(id).cost();
                    INFO ("  Original cost:    " << orig_cost);
                    INFO ("Restricted cost:    " << rest_cost);
                    REQUIRE (10.0 == Approx((rest_cost - orig_cost)));
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
