/*
 * JsonTestProvider.cc
 *
 *  Created on: 2015-05-18
 *      Author: Jonas Bergman
 */

#include "JsonTestProvider.h"  // class implemented
#include "../../graph/Vertex.h"
#include "../../graph/Edge.h"
#include "../../graph/TopoEdgeData.h"

#include <iostream>
#include <sstream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================

JsonTestProvider::JsonTestProvider(const Configuration& rConfig)
    : MapProvider(rConfig)
{
}



JsonTestProvider::~JsonTestProvider()
{
}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
void
JsonTestProvider::getTopology(Topology& rTopology)
{
    using namespace boost::property_tree;

    const std::string& filename = mrConfig.getTopologyConfig().testFile;
    ptree              pt;

    try
    {
        read_json(filename, pt);

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
//            Vertex v(v_row[0], Point(v_row[1], v_row[2]));
//            rTopology.addVertex(v);
            rTopology.addVertex(v_row[0], Point(v_row[1], v_row[2]));
        }

        // edges
        int e_row[4];
        for(auto& row : pt.get_child("edges"))
        {
            int i = 0;
            for(auto& item : row.second)
            {
                e_row[i] = item.second.get_value<int>();
                ++i;
            }
            TopoEdgeData::Direction direction;
            switch(e_row[3])
            {
                case 0:
                    direction = TopoEdgeData::Direction::BOTH; break;
                case 1:
                    direction = TopoEdgeData::Direction::FROM_TO; break;
                case 2:
                    direction = TopoEdgeData::Direction::TO_FROM; break;
                default:
                    direction = TopoEdgeData::Direction::BOTH;
            }
            Edge& e = rTopology.addEdge(e_row[0], e_row[1], e_row[2]);
            TopoEdgeData* p_ed = new TopoEdgeData();
            p_ed->setDirection(direction);
            e.setEdgeData(p_ed);
//            rTopology.addEdge(e);
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw TopologyException("Could not read file " + filename);
    }
}


//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


