/*
 * JsonTestProvider.cc
 *
 *  Created on: 2015-05-18
 *      Author: Jonas Bergman
 */

#include "JsonTestProvider.h"  // class implemented
#include "../../graph/Vertex.h"
#include "../../graph/Edge.h"

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
//JsonTestProvider::getMapData(Topology& rTopology)
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
            Edge::DirectionType direction;
            OsmIdType osm_id(std::numeric_limits<OsmIdType>::max());
            switch(e_row[3])
            {
                case 0:
                    direction = Edge::DirectionType::BOTH; break;
                case 1:
                    direction = Edge::DirectionType::FROM_TO; break;
                case 2:
                    direction = Edge::DirectionType::TO_FROM; break;
                default:
                    direction = Edge::DirectionType::BOTH;
            }
            Edge& e = rTopology.addEdge(e_row[0], osm_id, e_row[1], e_row[2]);
            Edge::RoadData rd;
            rd.direction = direction;
            e.setRoadData(rd);
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        throw TopologyException("Could not read file " + filename);
    }
}

void
JsonTestProvider::setRestrictionsAndCosts(Topology& rTopology)
{
    //none
}

void
JsonTestProvider::persistLineGraph(const Graph& rGraph)
{
    throw MapProviderException("JsonTestProvider has not "
        "implemented persisting a Line graph");
}

//============================= ACESS      ===================================
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


