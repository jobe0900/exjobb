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
//TopologyGraph::addVertex(TopologyVertex vertex)
//{
//    VertexType v = boost::add_vertex(vertex, mGraph);
//    mVertexMap.emplace(vertex.id(), v);
//}

void
TopologyGraph::addVertex(VertexData vertex)
{
    VertexType v = boost::add_vertex(vertex, mGraph);
    mGraph[v].topo_id   = vertex.topo_id;
    mGraph[v].x         = vertex.x;
    mGraph[v].y         = vertex.y;
    mVertexMap.emplace(vertex.topo_id, v);
}

//void
//TopologyGraph::addEdge(TopologyEdge edge)
//{
//    try
//    {
//        auto source_it = mVertexMap.find(edge.source());
//        if(source_it == mVertexMap.end())
//        {
//            throw TopologyException("Source vertex missing.");
//        }
//        auto target_it = mVertexMap.find(edge.target());
//        if(target_it == mVertexMap.end())
//        {
//            throw TopologyException("Target vertex missing.");
//        }
//        auto edge_add = boost::add_edge(source_it->second, target_it->second, edge, mGraph);
//        if(edge_add.second == true)
//        {
//            mEdgeMap.emplace(edge.id(), edge_add.first);
//        }
//    }
//    catch (TopologyException& e)
//    {
//        throw TopologyException("Cannot add edge: " + std::to_string(edge.id()) +
//            ". " + e.what());
//    }
//}

void
TopologyGraph::addEdge(EdgeData edge)
{
    try
    {
        auto source_it = mVertexMap.find(edge.source);
        if(source_it == mVertexMap.end())
        {
            throw TopologyException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(edge.target);
        if(target_it == mVertexMap.end())
        {
            throw TopologyException("Target vertex missing.");
        }
        auto edge_add = boost::add_edge(source_it->second, target_it->second, edge, mGraph);
        if(edge_add.second == true)
        {
            mGraph[edge_add.first].topo_id = edge.topo_id;
            mGraph[edge_add.first].source  = edge.source;
            mGraph[edge_add.first].target  = edge.target;
            mEdgeMap.emplace(edge.topo_id, edge_add.first);
        }
    }
    catch (TopologyException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(edge.topo_id) +
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


