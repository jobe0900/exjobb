/*
 * EdgeCost.cc
 *
 *      Author: Jonas Bergman
 */

#include "EdgeCost.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
void
EdgeCost::addCost(CostType type, Cost cost)
{
    if(hasCost(type))
    {
        if(type != EdgeCost::TRAVEL)
        {
            cost += getCost(type);
        }
        costs.erase(type);
    }
    costs.insert({type, cost});
}
//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
Cost
EdgeCost::getCost() const
{
    Cost sum {0};
    for(const auto& pair : costs)
    {
        sum += pair.second;
    }
    return sum;
}

Cost
EdgeCost::getCost(EdgeCost::CostType type) const
{
    const auto& it = costs.find(type);
    if(it != costs.end())
    {
        return it->second;
    }
    return 0;
}

//============================= INQUIRY    ===================================
bool
EdgeCost::hasCost(EdgeCost::CostType type) const
{
    return costs.find(type) != costs.end();
}
/////////////////////////////// PROTECTED  ///////////////////////////////////
/////////////////////////////// PRIVATE    ///////////////////////////////////


