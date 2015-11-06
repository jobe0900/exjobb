/*
 * Topology.cc
 *
 * @author  Jonas Bergman
 */

#include "Topology.h"  // class implemented

#include <utility>

/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
Topology::Topology()
    : mVertexMap(), mEdgeMap(), mOsmEdgeMap()
{
}
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================

Vertex&
Topology::addVertex(VertexIdType id, Point point)
{
    auto res = mVertexMap.emplace(id, Vertex(id, point));
    return res.first->second;
}


Edge&
Topology::addEdge(EdgeIdType        id,
                  OsmIdType         osmId,
                  VertexIdType      source,
                  VertexIdType      target,
                  Edge::GeomData    geomData,
                  Edge::RoadData    roadData)
{
     try
    {
        getVertex(source);
        getVertex(target);
        Edge edge(id, osmId, source, target, geomData, roadData);
        auto res = mEdgeMap.emplace(id, std::move(edge));
        mOsmEdgeMap.insert({osmId, id});
        return res.first->second;
    }
    catch (TopologyException& e)
    {
        throw TopologyException("Cannot add edge: " + std::to_string(id) +
                ". " + e.what());
    }
}

Edge&
Topology::addEdge(EdgeIdType        id,
                  OsmIdType         osmId,
                  VertexIdType      source,
                  VertexIdType      target)
{
    Edge::GeomData gd;
    Edge::RoadData rd;
    return addEdge(id, osmId, source, target, gd, rd);
}

Vertex&
Topology::getVertex(VertexIdType id)
{
    auto it = mVertexMap.find(id);
    if(it == mVertexMap.end()) {
        throw TopologyException("Vertex not found: " + std::to_string(id));
    }
    return it->second;
}

const Vertex&
Topology::getVertex(VertexIdType id) const
{
    return const_cast<Topology&>(*this).getVertex(id);
}

Edge&
Topology::getEdge(EdgeIdType id)
{
    auto it = mEdgeMap.find(id);
    if(it == mEdgeMap.end()) {
        throw TopologyException("Edge not found: " + std::to_string(id));
    }
    return it->second;
}

const Edge&
Topology::getEdge(EdgeIdType id) const
{
    auto it = mEdgeMap.find(id);
    if(it == mEdgeMap.end()) {
        throw TopologyException("Edge not found: " + std::to_string(id));
    }
    return it->second;
}

void
Topology::clearTopology()
{
    mVertexMap.clear();
    mEdgeMap.clear();
    mOsmEdgeMap.clear();
}

void
Topology::clearEdgeCostAndRestrictions()
{
    for(auto& it : mEdgeMap)
    {
        it.second.clearCostsAndRestrictions();
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

