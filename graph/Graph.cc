/*
 * Graph.cc
 *
 *  Created on: 2015-05-16
 *      Author: Jonas Bergman
 */

#include "Graph.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Graph::Graph(const Topology& rTopology)
    : mGraph(rTopology.nrVertices()),
      mVertexMap(),
      mEdgeMap(),
      mrTopology(rTopology)
{
}

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
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

