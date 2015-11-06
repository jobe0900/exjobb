///*
// * JsonTestProvider_test.cc
// *
// * @author  Jonas Bergman
// */
//
//#include "../JsonTestProvider.h"
//
//#include <string>
//#include <vector>
//
//#include "../../../catchtest/catch.hpp"
//#include "../../../config/ConfigurationReader.h"
//#include "../../../config/DatabaseConfig.h"
//#include "../../../graph/Edge.h"
//#include "../../../graph/Vertex.h"
//#include "../../../graph/Graph.h"
//
//SCENARIO ("JsonTest topology handling", "[jsontest]")
//{
//    try
//    {
//        // ===================================================================
//        GIVEN ("a valid configuration structure with a jsontest filename")
//        {
//            std::string config_file("mapprovider/jsontest"
//                "/catchtest/jsontest-settings.json");
//            ConfigurationReader config_reader(config_file);
//            Configuration config;
//            config_reader.fillConfiguration(config);
//            JsonTestProvider* p_jt(nullptr);
//
//            // ...............................................................
//            WHEN ("we try to create topology")
//            {
//                THEN ("we should not receive an exception")
//                {
//                    REQUIRE_NOTHROW ( p_jt = new JsonTestProvider(config));
//                }
//            }
//
//            // ...............................................................
//            WHEN ("we try to fetch topology ")
//            {
//                Topology topology;
//                JsonTestProvider jtp(config);
//
//                // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//                THEN ("we should not receive an exception")
//                {
//                    REQUIRE_NOTHROW (jtp.getTopology(topology));
//                }
//            }
//            // ...............................................................
//            WHEN ("using topology")
//            {
//                Topology topology;
//                JsonTestProvider jtp(config);
//                jtp.getTopology(topology);
//
//                size_t nr_vertices = 13;
//                size_t nr_edges = 16;
//
//                Configuration config;
//
//                // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//                THEN ("we should have the right number of edges and vertices"
//                    " in the topology")
//                {
//                    REQUIRE (topology.nrVertices() == nr_vertices);
//                    REQUIRE (topology.nrEdges() == nr_edges);
//                }
//
//                // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//                THEN ("we should be able to create a graph from topology")
//                {
//                    REQUIRE_NOTHROW (Graph graph(topology, config));
//                }
//
//                // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//                THEN ("we should be able to create a graph from topology"
//                    " and print out the graph")
//                {
//                    Graph graph(topology, config);
//                    INFO (graph);
//                    REQUIRE (true);
//                }
//            }
//            delete p_jt;
//        }
//
//    }
//    catch (ConfigurationException& e) {
//        INFO(e.what());
//        REQUIRE (false);    // force output of error and failure
//    }
//}
//
//
