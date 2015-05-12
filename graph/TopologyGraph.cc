/*
 * TopologyGraph.cc
 *
 *  Created on: 2015-05-12
 *      Author: Jonas Bergman
 */

#include "TopologyGraph.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
TopologyGraph::TopologyGraph()
    : mGraph()//, mNrVertices(0), mNrEdges(0)
{
}

TopologyGraph::TopologyGraph(size_t nrVertices)
    : mGraph(nrVertices)//, mNrVertices(0), mNrEdges(0)
{
}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
void
TopologyGraph::addVertex(const TopologyVertex& rVertex)
{
    VertexType v = boost::add_vertex(rVertex, mGraph);
    auto& res = mVertexMap.emplace(rVertex.id(), v);
//    if(res.second == true)
//    {
//        ++mNrVertices;
//    }
}

void
TopologyGraph::addEdge(const TopologyEdge& rEdge)
{
    try
    {
        auto source_it = mVertexMap.find(rEdge.source().id());
        if(source_it == mVertexMap.end())
        {
            throw TopologyException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(rEdge.target().id());
        if(target_it == mVertexMap.end())
        {
            throw TopologyException("Target vertex missing.");
        }
        auto& edge_add = boost::add_edge(*source_it, *target_it, rEdge, mGraph);
        if(edge_add.second == true)
        {
            auto& res = mEdgeMap.emplace(rEdge.id(), edge_add.first);
//            if(res.second == true)
//            {
//                ++mNrEdges
//            }
        }
    }
    catch (TopologyException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(rEdge.id()) +
            ". " + e.what());
    }
}


//============================= ACESS      ===================================
size_t
TopologyGraph::nrVertices() const
{
    return mVertexMap.size();
}

size_t
TopologyGraph::nrEdges() const
{
    return mEdgeMap.size();
}
//============================= INQUIRY    ===================================
bool
TopologyGraph::hasVertex(VertexId vertexId) const
{
    auto& it = mVertexMap.find(vertexId);
    return (it != mVertexMap.end());
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


