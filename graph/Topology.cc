/*
 * Topology.cc
 *
 *  Created on: 2015-05-11
 *      Author: Jonas Bergman
 */

#include "Topology.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Topology::Topology()
{
}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
const TopologyVertex&
Topology::addVertex(TopologyId id, Point point)
{
    auto res = mVertexMap.emplace(id, TopologyVertex(id, point));
    return res.first->second;
}


const TopologyEdge&
Topology::addEdge(EdgeId id, VertexId source, VertexId target)
{
    try
    {
        const TopologyVertex& r_source = getVertex(source);
        const TopologyVertex& r_target = getVertex(target);
        auto res = mEdgeMap.emplace(id, TopologyEdge(id, r_source, r_target));
        return res.first->second;
    }
    catch (TopologyException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(id) +
            ". " + e.what());
    }
}


const TopologyVertex&
Topology::getVertex(VertexId id) const
{
    auto it = mVertexMap.find(id);
    if(it == mVertexMap.end()) {
        throw TopologyException("Vertex not found: " + std::to_string(id));
    }
    return it->second;
}


void
Topology::buildTopologyGraph(TopologyGraph& rGraph)
{
    for(auto& edge_it : mEdgeMap) {
        boost::add_edge(edge_it.second.source().id(), edge_it.second.target().id(), rGraph);
    }
}

//============================= ACESS      ===================================
size_t
Topology::nrVertices() const
{
    return mVertexMap.size();
}

size_t
Topology::nrEdges() const
{
    return mEdgeMap.size();
}
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////




