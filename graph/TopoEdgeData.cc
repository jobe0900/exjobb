/*
 * TopoEdgeData.cc
 *
 *  Created on: 2015-05-19
 *      Author: Jonas Bergman
 */

#include "TopoEdgeData.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
TopoEdgeData::TopoEdgeData()
    : mDirection(TopoEdgeData::Direction::BOTH),
      mNrLanes(1),
      mLength(1.0)
{}

TopoEdgeData::~TopoEdgeData()
{}
//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
void
TopoEdgeData::setDirection(TopoEdgeData::Direction direction)
{
    mDirection = direction;
}

void
TopoEdgeData::setNrLanes(size_t nrLanes)
{
    mNrLanes = nrLanes;
}

void
TopoEdgeData::setLength(double length)
{
    mLength = length;
}

//============================= ACESS      ===================================
TopoEdgeData::Direction
TopoEdgeData::direction() const
{ return mDirection; }

size_t
TopoEdgeData::nrLanes() const
{ return mNrLanes; }

double
TopoEdgeData::length() const
{ return mLength; }

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

void
TopoEdgeData::print(std::ostream& os) const
{
    os  << "direction: ";

    switch(mDirection)
    {
        case TopoEdgeData::BOTH:
            os << "BOTH"; break;
        case TopoEdgeData::FROM_TO:
            os << "FROM_TO"; break;
        case TopoEdgeData::TO_FROM:
            os << "TO_FROM"; break;
    }

    os << ", #lanes: " << mNrLanes;
}

/////////////////////////////// PRIVATE    ///////////////////////////////////






