/*
 * Vertex.cc
 *
 * @author  Jonas Bergman
 */

#include "Vertex.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////
const VertexIdType Vertex::MAX_ID = std::numeric_limits<VertexIdType>::max();

//============================= LIFECYCLE ====================================
Vertex::Vertex(VertexIdType id, Point point)
    : mId(id), mPoint(point)
{}

//============================= OPERATORS ====================================
std::ostream&
operator<<(std::ostream& os, const Vertex& rVertex)
{
    os  << "Vertex [id: " << rVertex.mId
        << ", point: " << rVertex.mPoint << "]";
    return os;
}

bool
Vertex::operator==(const Vertex& rhs) const
{
    return (rhs.mId == mId) && (rhs.mPoint == mPoint);
}

//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
VertexIdType
Vertex::id() const
{ return mId; }

Point
Vertex::point() const
{ return mPoint; }

//============================= INQUIRY    ===================================
bool
Vertex::hasRestrictions() const
{ return false; }

/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
