/*
 * OsmTurningRestriction.cc
 *
 *  Created on: 2015-05-25
 *      Author: Jonas Bergman
 */


#include "OsmTurningRestriction.h"  // class implemented


/////////////////////////////// PUBLIC ///////////////////////////////////////

//============================= LIFECYCLE ====================================
OsmTurningRestriction::OsmTurningRestriction(
    OsmTurningRestriction::TurningRestrictionType   type,
    EdgeIdType                                      fromEdgeId,
    VertexIdType                                    viaVertexId,
    EdgeIdType                                      toEdgeId)
    : mType(type),
      mFromEdgeId(fromEdgeId),
      mViaType(VIA_NODE),
      mViaEdgeIds(),
      mViaVertexId(viaVertexId),
      mToEdgeId(toEdgeId)
{}

OsmTurningRestriction::OsmTurningRestriction(
    OsmTurningRestriction::TurningRestrictionType   type,
    EdgeIdType                                      fromEdgeId,
    std::vector<EdgeIdType>                         viaEdgeIds,
    EdgeIdType                                      toEdgeId)
    : mType(type),
      mFromEdgeId(fromEdgeId),
      mViaType(VIA_WAY),
      mViaEdgeIds(viaEdgeIds),
      mViaVertexId(),
      mToEdgeId(toEdgeId)
{}

//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//static
OsmTurningRestriction::TurningRestrictionType
OsmTurningRestriction::parseString(const std::string& rTypeString)
{
    for(size_t i = 0; i < sTypeStrings.size(); ++i)
    {
        if(rTypeString == OsmTurningRestriction::sTypeStrings[i])
        {
            return static_cast<TurningRestrictionType>(i);
        }
    }
    throw OsmException(
        "OsmTurningRestriction:parseString: "
        "Unknown TurningRestriction Type string.");
}

//static
std::string
OsmTurningRestriction::toString(
    OsmTurningRestriction::TurningRestrictionType turnRestrictionType)
{
    if(turnRestrictionType >= sTypeStrings.size())
    {
        throw OsmException(
            "OsmTurningRestriction:toString: Unknown TurningRestriction Type");
    }
    return OsmTurningRestriction::sTypeStrings[turnRestrictionType];
}

std::string
OsmTurningRestriction::toString() const
{
    std::ostringstream oss;
    oss << sTypeStrings[this->mType] << ": "
        << "from: " << mFromEdgeId;

    // via vertex
    if(mViaType == VIA_NODE)
    {
        oss << ", via vertex: " << mViaVertexId;
    }
    else // via edges
    {
        oss << ", via edges: | ";
        for(EdgeIdType id : mViaEdgeIds)
        {
            oss << id << " | ";
        }
//        oss << "]";
    }

    oss << ", to: " << mToEdgeId;

    return oss.str();
}

std::string
OsmTurningRestriction::typeToString() const
{
    return sTypeStrings[this->mType];
}
//============================= ACESS      ===================================
EdgeIdType
OsmTurningRestriction::fromEdgeId() const
{
    return mFromEdgeId;
}

OsmTurningRestriction::TurningViaType
OsmTurningRestriction::viaType() const
{
    return mViaType;
}

const std::vector<EdgeIdType>&
OsmTurningRestriction::viaEdgeIds() const
{
    return mViaEdgeIds;
}

VertexIdType
OsmTurningRestriction::viaVertexId() const
{
    return mViaVertexId;
}

EdgeIdType
OsmTurningRestriction::toEdgeId() const
{
    return mToEdgeId;
}
//============================= INQUIRY    ===================================
bool
OsmTurningRestriction::isInRestriction(EdgeIdType edgeId) const
{
    if(edgeId == mFromEdgeId
    || edgeId == mToEdgeId)
    {
        return true;
    }

    auto it = std::find(mViaEdgeIds.begin(), mViaEdgeIds.end(), edgeId);
    if(it != mViaEdgeIds.end())
    {
        return true;
    }

    return false;
}
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
// static
std::vector<std::string> OsmTurningRestriction::sTypeStrings
{
    "none",
    "no_left_turn",
    "no_right_turn",
    "no_straight_on",
    "no_u_turn",
    "only_right_turn",
    "only_left_turn",
    "only_straight_on",
    "no_entry",
    "no_exit"
};


