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
    : vertexMap(), edgeMap()
{
}
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//const Vertex&
//Topology::addVertex(VertexIdType id, Point point)
//{
//    auto res = vertexMap.emplace(id, Vertex(id, point));
//    return res.first->second;
//}

Vertex&
Topology::addVertex(VertexIdType id, Point point)
{
    auto res = vertexMap.emplace(id, Vertex(id, point));
    return res.first->second;
}

//const Vertex&
//Topology::addVertex(Vertex vertex)
//{
//    auto res = vertexMap.emplace(vertex.id(), vertex);
//    return res.first->second;
//}


//const Edge&
//Topology::addEdge(EdgeIdType id, VertexIdType source, VertexIdType target)
//{
// 	try
//	{
//	    getVertex(source);
//	    getVertex(target);
//		auto res = edgeMap.emplace(id, Edge(id, source, target));
//		return res.first->second;
//	}
//	catch (TopologyException& e)
//	{
//		throw TopologyException("Cannot add edge: " + std::to_string(id) +
//				". " + e.what());
//	}
//}

Edge&
Topology::addEdge(EdgeIdType id, VertexIdType source, VertexIdType target)
{
 	try
	{
	    getVertex(source);
	    getVertex(target);
		auto res = edgeMap.emplace(id, Edge(id, source, target));
//		auto res = edgeMap.emplace(id, id, source, target);
		return res.first->second;
	}
	catch (TopologyException& e)
	{
		throw TopologyException("Cannot add edge: " + std::to_string(id) +
				". " + e.what());
	}
}

//const Edge&
//Topology::addEdge(Edge edge)
//{
//    try
//    {
//        getVertex(edge.source());
//        getVertex(edge.target());
//        auto res = edgeMap.emplace(edge.id(), edge);
//        return res.first->second;
//    }
//    catch (TopologyException& e)
//    {
//        throw TopologyException("Cannot add edge: " + std::to_string(edge.id()) +
//                ". " + e.what());
//    }
//}


const Vertex&
Topology::getVertex(VertexIdType id) const
{
    auto it = vertexMap.find(id);
    if(it == vertexMap.end()) {
        throw TopologyException("Vertex not found: " + std::to_string(id));
    }
    return it->second;
}

Vertex&
Topology::getVertex(VertexIdType id)
{
    auto it = vertexMap.find(id);
    if(it == vertexMap.end()) {
        throw TopologyException("Vertex not found: " + std::to_string(id));
    }
    return it->second;
}

const Edge&
Topology::getEdge(EdgeIdType id) const
{
    auto it = edgeMap.find(id);
    if(it == edgeMap.end()) {
        throw TopologyException("Edge not found: " + std::to_string(id));
    }
    return it->second;
}

Edge&
Topology::getEdge(EdgeIdType id)
{
    auto it = edgeMap.find(id);
    if(it == edgeMap.end()) {
        throw TopologyException("Edge not found: " + std::to_string(id));
    }
    return it->second;
}

//============================= ACESS      ===================================
size_t
Topology::nrVertices() const
{
    return vertexMap.size();
}

size_t
Topology::nrEdges() const
{
    return edgeMap.size();
}
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////

