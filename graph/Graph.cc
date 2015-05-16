/*
 * Graph.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "Graph.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
//GGraph::Graph()
//    : mGraph()//, mNrVertices(0), mNrEdges(0)
//{
//}

Graph::Graph(const Topology& rTopology)
    : mGraph(rTopology.nrVertices()),
      mVertexMap(),
      mEdgeMap(),
      mrTopology(rTopology)
{
}

//TopologyGraph::~TopologyGraph()
//{
//    // need to delete pointers here?
//}


//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================

//void
//TopologyGraph::addVertex(TopologyVertex vertex)
//{
//    VertexType v = boost::add_vertex(vertex, mGraph);
//    mVertexMap.emplace(vertex.id(), v);
//}

void
Graph::addVertex(const Vertex& vertex)
{
    VertexType v = boost::add_vertex(mGraph);
    mVertexMap.emplace(vertex.id(), v);
}

void
Graph::addEdge(const Edge& edge)
{
    try
    {
        auto source_it = mVertexMap.find(edge.source());
        if(source_it == mVertexMap.end())
        {
            throw GraphException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(edge.target());
        if(target_it == mVertexMap.end())
        {
            throw GraphException("Target vertex missing.");
        }
        auto edge_add = boost::add_edge(source_it->second, target_it->second, mGraph);
        if(edge_add.second == true)
        {
            mEdgeMap.emplace(edge.id(), edge_add.first);
        }
    }
    catch (GraphException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(edge.id()) +
            ". " + e.what());
    }
}


//============================= ACESS      ===================================
size_t
Graph::nrVertices() const
{
    return mVertexMap.size();
}

size_t
Graph::nrEdges() const
{
    return mEdgeMap.size();
}

const Graph::GraphType&
Graph::getRepresentation() const
{
    return mGraph;
}

//============================= INQUIRY    ===================================
bool
Graph::hasVertex(VertexIdType vertexId) const
{
    const auto& it = mVertexMap.find(vertexId);
    return (it != mVertexMap.end());
}

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////


