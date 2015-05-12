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
    // need to delete pointers here?
}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//void
//TopologyGraph::addVertex(const TopologyVertex* pVertex)
//{
//    VertexType v = boost::add_vertex(*pVertex, mGraph);
//    /*auto& res =*/ mVertexMap.emplace(pVertex->id(), v);
////    if(res.second == true)
////    {
////        ++mNrVertices;
////    }
////    delete pVertex;
//}

void
TopologyGraph::addVertex(TopologyVertex vertex)
{
    VertexType v = boost::add_vertex(vertex, mGraph);
    /*auto& res =*/ mVertexMap.emplace(vertex.id(), v);
//    if(res.second == true)
//    {
//        ++mNrVertices;
//    }
//    delete pVertex;
}

//void
//TopologyGraph::addEdge(const TopologyEdge* pEdge)
//{
//    try
//    {
//        auto source_it = mVertexMap.find(pEdge->source());
//        if(source_it == mVertexMap.end())
//        {
//            throw TopologyException("Source vertex missing.");
//        }
//        auto target_it = mVertexMap.find(pEdge->target());
//        if(target_it == mVertexMap.end())
//        {
//            throw TopologyException("Target vertex missing.");
//        }
//        auto& edge_add = boost::add_edge(*source_it, *target_it, mGraph);
////        auto& edge_add = boost::add_edge(*source_it, *target_it, *pEdge, mGraph);
//        if(edge_add.second == true)
//        {
//            auto& res = mEdgeMap.emplace(pEdge->id(), edge_add.first);
////            if(res.second == true)
////            {
////                ++mNrEdges
////            }
//        }
//        delete pEdge;
//    }
//    catch (TopologyException& e)
//    {
//        delete pEdge;
//        throw TopologyException("Cannot add edge: " + std::to_string(pEdge->id()) +
//            ". " + e.what());
//    }
//
//}

void
TopologyGraph::addEdge(TopologyEdge edge)
{
    try
    {
        auto source_it = mVertexMap.find(edge.source());
        if(source_it == mVertexMap.end())
        {
            throw TopologyException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(edge.target());
        if(target_it == mVertexMap.end())
        {
            throw TopologyException("Target vertex missing.");
        }
//        auto edge_add = boost::add_edge(source_it->second, target_it->second, mGraph);
        auto edge_add = boost::add_edge(source_it->second, target_it->second, edge, mGraph);
        if(edge_add.second == true)
        {
            /*auto res =*/ mEdgeMap.emplace(edge.id(), edge_add.first);
//            if(res.second == true)
//            {
//                ++mNrEdges
//            }
        }
    }
    catch (TopologyException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(edge.id()) +
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
    const auto& it = mVertexMap.find(vertexId);
    return (it != mVertexMap.end());
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


