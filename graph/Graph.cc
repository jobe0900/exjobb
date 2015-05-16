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
    for(Vertex& vertex : rTopology.mrVertexMap)
    {
        addVertex(vertex);
    }
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

void
Graph::addVerticesToGraph(const Vertex& rVertex)
{
    VertexType v = boost::add_vertex(mGraph);
    mVertexMap.emplace(rVertex.id(), v);
}

void
Graph::addEdgesToGraph(const Edge& rEdge)
{
    try
    {
        auto source_it = mVertexMap.find(rEdge.source());
        if(source_it == mVertexMap.end())
        {
            throw GraphException("Source vertex missing.");
        }
        auto target_it = mVertexMap.find(rEdge.target());
        if(target_it == mVertexMap.end())
        {
            throw GraphException("Target vertex missing.");
        }
        auto edge_add = boost::add_edge(source_it->second, target_it->second, mGraph);
        if(edge_add.second == true)
        {
            mEdgeMap.emplace(rEdge.id(), edge_add.first);
        }
    }
    catch (GraphException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(rEdge.id()) +
            ". " + e.what());
    }
}

