/*
 * LineGraphEdgeData.cc
 *
 *  Created on: 2015-05-19
 *      Author: Jonas Bergman
 */

#include "LineGraphEdgeData.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
LineGraphEdgeData::LineGraphEdgeData()
    : mCost(1.0), mViaVertexId()
{}

LineGraphEdgeData::~LineGraphEdgeData()
{}

//============================= OPERATORS ====================================

//============================= OPERATIONS ===================================
void
LineGraphEdgeData::setCost(double cost)
{
    mCost = cost;
}

void
LineGraphEdgeData::setViaVertexId(VertexIdType vertexId)
{
    mViaVertexId = vertexId;
}

//============================= ACESS      ===================================
double
LineGraphEdgeData::cost() const
{ return mCost; }

VertexIdType
LineGraphEdgeData::viaVertexId() const
{ return mViaVertexId; }

//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

void
LineGraphEdgeData::print(std::ostream& os) const
{
    os  << "cost: " << mCost << ", via vertex: " << mViaVertexId;
}

/////////////////////////////// PRIVATE    ///////////////////////////////////









