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
		auto res = mEdgeMap.emplace(
		    id, Edge(id, osmId, source, target, geomData, roadData));
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
    return const_cast<Topology&>(*this).getEdge(id);
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

