/*
 * GraphBuilder_test.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../../catchtest/catch.hpp"
#include "../TopologyGraph.h"

void loadSampleData(TopologyGraph& topo);

SCENARIO ("Building a sample BGL graph", "[graph]")
{
    // -----------------------------------------------------------------------
    GIVEN ("a sample data")
    {


        WHEN ("we try to build a graph")
        {
            TopologyGraph graph;
            loadSampleData(graph);
            const int nr_vertices = 13;
            const int nr_edges = 16;
            REQUIRE (graph.nrVertices() == nr_vertices);
            REQUIRE (graph.nrEdges() == nr_edges);


            THEN ("we should get a reference to a Graph object")
            {
                const TopologyGraph::GraphType& r_graph = graph.getRepresentation();

                auto vertex_it = boost::vertices(r_graph);
                auto vid_map = boost::get(&VertexData::topo_id, r_graph);
                auto x_map  = boost::get(&VertexData::x, r_graph);
                auto y_map  = boost::get(&VertexData::y, r_graph);
                for(; vertex_it.first != vertex_it.second; ++vertex_it.first)
                {
                    auto ix = *vertex_it.first;
                    std::cout << "vertex:" << ix
                              << "\tid: " << vid_map[ix]
                              << "\tx: " << x_map[ix]
                              << "\ty: " << y_map[ix]
                              << std::endl;
                }

                auto edge_it = boost::edges(r_graph);
                auto eid_map    = boost::get(&EdgeData::topo_id, r_graph);
                auto source_map = boost::get(&EdgeData::source, r_graph);
                auto target_map = boost::get(&EdgeData::target, r_graph);
                for(; edge_it.first != edge_it.second; ++edge_it.first)
                {
                    auto ix = *edge_it.first;
                    std::cout << "edge:" << ix
                              << "\tid: " << eid_map[ix]
                              << "\tx: " << source_map[ix]
                              << "\ty: " << target_map[ix]
                              << std::endl;
                }
            }
        }
    }
}

void loadSampleData(TopologyGraph& graph)
{
    using namespace boost::property_tree;

    std::string     sample_json("graph/catchtest/test-topology.json");
    ptree           pt;

    try
    {
        read_json(sample_json, pt);

        // vertices
        int v_row[3];
        for(auto& row : pt.get_child("vertices"))
        {
            int i = 0;
            for(auto& item : row.second)
            {
                v_row[i] = item.second.get_value<int>();
                ++i;
            }
            VertexData v(v_row[0], v_row[1], v_row[2]);
            graph.addVertex(v);
            std::cout << "vertex [id: " << v_row[0]
                      << ", x: " << v_row[1]
                      << ", y: " << v_row[2] << "]" << std::endl;
        }

        // edges
        int e_row[3];
        for(auto& row : pt.get_child("edges"))
        {
            int i = 0;
            for(auto& item : row.second)
            {
                e_row[i] = item.second.get_value<int>();
                ++i;
            }
            EdgeData e(e_row[0], e_row[1], e_row[2]);
            graph.addEdge(e);
            std::cout << "edge [id: " << e_row[0]
                      << ", s: " << e_row[1]
                      << ", t: " << e_row[2] << "]" << std::endl;
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw TopologyException("Could not read file " + sample_json);
    }
}
