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

TopologyGraph::~TopologyGraph()
{
    // need to delete pointers here
}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
void
TopologyGraph::addVertex(const TopologyVertex* pVertex)
{
    VertexType v = boost::add_vertex(*pVertex, mGraph);
    auto& res = mVertexMap.emplace(pVertex->id(), v);
//    if(res.second == true)
//    {
//        ++mNrVertices;
//    }
    delete pVertex;
}

void
TopologyGraph::addEdge(const TopologyEdge* pEdge)
{
    try
    {
        auto source_it = mVertexMap.find(pEdge->source().id());
        if(source_it == mVertexMap.end())
        {
            throw TopologyException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(pEdge->target().id());
        if(target_it == mVertexMap.end())
        {
            throw TopologyException("Target vertex missing.");
        }
        auto& edge_add = boost::add_edge(*source_it, *target_it, *pEdge, mGraph);
        if(edge_add.second == true)
        {
            auto& res = mEdgeMap.emplace(pEdge->id(), edge_add.first);
//            if(res.second == true)
//            {
//                ++mNrEdges
//            }
        }
        delete pEdge;
    }
    catch (TopologyException& e)
    {
        delete pEdge;
        throw TopologyException("Cannot add edge: " + std::to_string(pEdge->id()) +
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

const TopologyGraph::GraphType&
TopologyGraph::getRepresentation() const
{
    return mGraph;
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


