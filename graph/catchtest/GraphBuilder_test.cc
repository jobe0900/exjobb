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
#include "../Topology.h"

void loadSampleData(Topology& topo);

SCENARIO ("Building a sample BGL graph", "[graph]")
{
    // -----------------------------------------------------------------------
    GIVEN ("a Topology object with sample data")
    {
        Topology topo;
        loadSampleData(topo);

        REQUIRE (topo.nrVertices() == 13);
        REQUIRE (topo.nrEdges() == 16);

        WHEN ("we try to build a graph")
        {
            THEN ("we should get a reference to a Graph object")
            {

            }
        }
    }
}

void loadSampleData(Topology& topo)
{
    using namespace boost::property_tree;

    std::string sample_json("graph/catchtest/test-topology.json");
    ptree       pt;

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
            topo.addVertex(v_row[0], Point(v_row[1], v_row[2]));
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
            topo.addEdge(e_row[0], e_row[1], e_row[2]);
            std::cout << "edge [id: " << e_row[0]
                      << ", x: " << e_row[1]
                      << ", y: " << e_row[2] << "]" << std::endl;
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw TopologyException("Could not read file " + sample_json);
    }
}
