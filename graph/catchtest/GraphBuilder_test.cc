/*
 * Graph_test.cc
 *
 * @author  Jonas Bergman
 */

#include <iostream>

#include "../GraphBuilder.h"
#include "../Topology.h"
#include "../../catchtest/catch.hpp"
#include "../../config/ConfigurationReader.h"
#include "../../mapprovider/postgis/PostGisProvider.h"

SCENARIO ("Building a small graph", "[graph][basic]")
{
    // -----------------------------------------------------------------------
    GIVEN ("Three points and two edges for a topology")
    {
        size_t nr_vertices = 3;
        size_t nr_edges = 2;
        OsmIdType osm_id(1);

        Topology topology;
        const Vertex& v1 = topology.addVertex(1, Point(0,0));
        const Vertex& v2 = topology.addVertex(2, Point(1,2));
        const Vertex& v3 = topology.addVertex(3, Point(3,1));
        Edge& e1 = topology.addEdge(1,osm_id,1,2);
        Edge& e2 = topology.addEdge(2,osm_id,2,3);

        Configuration config;

        // ...................................................................
        WHEN ("we try create a Graph from the Topology")
        {
            THEN ("we should not get an Exception")
            {
                INFO ("calling Graph constructor");
                REQUIRE_NOTHROW (GraphBuilder g(topology, config));
            }
        }

        // ...................................................................
        WHEN ("building a graph from the topology")
        {
            GraphBuilder g(topology, config);
            const auto& boost_graph = g.getBoostGraph();
            LineGraphType& r_boost_line_graph = g.getBoostLineGraph();

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the # of vertices in the graph representation"
                " should be as in the topology"
                " and the # edges the double") // default is bidirectional
            {
                REQUIRE (boost::num_vertices(boost_graph) == nr_vertices);
                REQUIRE (boost::num_edges(boost_graph) == nr_edges * 2);
            }

            // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
            THEN ("the number of nodes in the LineGraph"
                  " should be as many as edges in the graph")
            {
                REQUIRE (boost::num_vertices(r_boost_line_graph) ==
                         boost::num_edges(boost_graph));
            }
        }

        // ...................................................................
        WHEN ("we try print out a Graph from the Topology")
        {
            GraphBuilder g(topology, config);

            THEN ("we should get a print out")
            {
                INFO (g);
                REQUIRE (true);
            }
        }

        // ...................................................................
        WHEN ("adding unidirectional information to edges before"
              " building graph")
        {

            Edge::RoadData rd1;
            rd1.direction = Edge::DirectionType::FROM_TO;
            e1.setRoadData(rd1);

            Edge::RoadData rd2;
            rd2.direction = Edge::DirectionType::FROM_TO;
            e2.setRoadData(rd2);

            GraphBuilder g2(topology, config);

            THEN ("the # of edges in the graph representation"
                  " should as many as in the topology")
            {
                INFO (g2);
                const auto& boost_graph = g2.getBoostGraph();
                REQUIRE (boost::num_edges(boost_graph) == topology.nrEdges());
            }
        }

        // ...................................................................
        WHEN ("adding an extra lane to an edge before"
              " building graph")
        {

            Edge::RoadData rd1;
            rd1.direction = Edge::DirectionType::FROM_TO;
            rd1.nrLanes = 2;
            e1.setRoadData(rd1);

            Edge::RoadData rd2;
            rd2.direction = Edge::DirectionType::FROM_TO;
            e2.setRoadData(rd2);

            GraphBuilder g2(topology, config);

            THEN ("the # of edges in the graph representation"
                " should be one more than in the topology")
            {
                INFO (g2);
                const auto& boost_graph = g2.getBoostGraph();
                REQUIRE (boost::num_edges(boost_graph) == topology.nrEdges() + 1);
            }
        }
    }
}

SCENARIO ("Building graph with restrictions", "[graph][restrictions]")
{
    try
    {

        // ===================================================================
        GIVEN ("Configuration to build a Graph with restrictions ")
        {
            std::string config_file(
                "catchtest/testsettings/mikh_restr_0617-testsettings.json");
            ConfigurationReader config_reader(config_file);
            Configuration config;
            config_reader.fillConfiguration(config);

            PostGisProvider pgp(config);

            Topology topology;
            pgp.getTopology(topology);
            pgp.setRestrictionsAndCosts(topology);

            GraphBuilder graph_restr(topology, config);

            GraphBuilder graph_unrestr(topology, config, false);

            // ...............................................................
            WHEN ("Adding a turning restriction and a point restriction (barrier)")
            {

                THEN ("there should be equally many vertices "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Vertices: " << graph_restr.nrVertices());
                    INFO ("UNRestricted # Vertices: " << graph_unrestr.nrVertices());
                    REQUIRE (graph_restr.nrVertices() == graph_unrestr.nrVertices());
                }

                THEN ("there should be 2 less edges "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Edges:    " << graph_restr.nrEdges());
                    INFO ("UNRestricted # Edges:    " << graph_unrestr.nrEdges());
                    REQUIRE (graph_restr.nrEdges() == graph_unrestr.nrEdges() - 2);
                }

                THEN ("there should be 2 less nodes "
                      "in restricted and unrestricted")
                {
                    INFO ("  Restricted # Nodes:    " << graph_restr.nrNodes());
                    INFO ("UNRestricted # Nodes:    " << graph_unrestr.nrNodes());
                    REQUIRE (graph_restr.nrNodes() == graph_unrestr.nrNodes() - 2);
                }
                THEN ("there should be 9 lines less "
                      "in restricted than unrestricted")
                {
                    // 1 turn restriction
                    // 3*2 where target is restricted by barrier (lift gate)
                    // 2 u-turns on restricted edge
                    INFO ("  Restricted # Lines:    " << graph_restr.nrLines());
                    INFO ("UNRestricted # Lines:    " << graph_unrestr.nrLines());
                    REQUIRE (graph_restr.nrLines() == graph_unrestr.nrLines() - 9);
                }
                THEN ("we can print the info for an edge and it should have a cost")
                {
                    EdgeIdType id = 270;
                    const Edge& edge = topology.getEdge(id);
                    INFO ("Edge " << id << ": " << edge);
                    REQUIRE (true);
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
