/**  Access to Turning restriction data from the OSM file.
 *
 * #include "OsmTurningRestriction.h"
 *
 * @author Jonas Bergman
 */

#ifndef OSM_OSMTURNINGRESTRICTION_H_
#define OSM_OSMTURNINGRESTRICTION_H_

// SYSTEM INCLUDES
//
#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "OsmException.h"
#include "../graph/Edge.h"
#include "../graph/Vertex.h"

// FORWARD REFERENCES
//

class OsmTurningRestriction
{
public:
// TYPES
    enum TurningRestrictionType
    {
        NONE,
        NO_LEFT_TURN,
        NO_RIGHT_TURN,
        NO_STRAIGHT_ON,
        NO_U_TURN,
        ONLY_RIGHT_TURN,
        ONLY_LEFT_TURN,
        ONLY_STRAIGHT_ON,
        NO_ENTRY,
        NO_EXIT,

        NR_TURNING_RESTRICTION_TYPES
    };

    enum TurningViaType
    {
        VIA_NODE,
        VIA_WAY
    };

// LIFECYCLE
    /** Constructor. Disabled*/
    OsmTurningRestriction() = delete;

    /** Constructor.
     * Turning restriction via a vertex.
     * @param   type        The type of turning restriction.
     * @param   fromEdgeId  The Edge the turn starts at.
     * @param   viaVertexId The Vertex the turn travels via.
     * @param   toEdgeId    The Edge the turn ends at.
     */
    OsmTurningRestriction(TurningRestrictionType    type,
                          EdgeIdType                fromEdgeId,
                          VertexIdType              viaVertexId,
                          EdgeIdType                toEdgeId);

    /** Constructor.
     * Turning restriction via  other Edges.
     * @param   type        The type of turning restriction.
     * @param   fromEdgeId  The Edge the turn starts at.
     * @param   viaOsmIds   String with the OsmIds of roads the turn travels via.
     * @param   toEdgeId    The Edge the turn ends at.
     */
    OsmTurningRestriction(TurningRestrictionType    type,
                          EdgeIdType                fromEdgeId,
                          std::string               viaOsmIds,
                          EdgeIdType                toEdgeId);

    /** Copy constructor. */
    OsmTurningRestriction(const OsmTurningRestriction& from) = default;

    /** Destructor. */
    ~OsmTurningRestriction() = default;

// OPERATORS
// OPERATIONS
    /** Attempt to parse a string to a TurningRestrictionType
     * @param   rTypeString     String which could contain a Turning Restriction
     * @return  A valid TurningRestrictionType
     * @throw   OsmException if invalid string.
     */
    static TurningRestrictionType  parseString(const std::string& rTypeString);

    /** Convert a Turning Restriction Type to a string representation.
     * @param   turnRestrictionType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown turn restriction type (out of bounds).
     */
    static std::string  toString(TurningRestrictionType turnRestrictionType);

    /** Convert this TurningRestriction to a string.
     * @return  string representation of this turning restriction.
     */
    std::string         toString() const;

    /** Convert this TurningRestrictions type to a string.
     * @return  string representation of this turning restriction type.
     */
    std::string         typeToString() const;

// ACCESS
    /**
     * @return  The Edge Id of the 'from' edge
     */
    EdgeIdType          fromEdgeId() const;

    /**
     * @return  The Via type, 'way' or 'node'.
     */
    TurningViaType      viaType() const;

    /**
     * @return  The Ids of the Edges in a 'via way' relation.
     */
    std::string         viaOsmIds() const;

    /**
     * @return  The Vertex Id of the 'via' vertex.
     */
    VertexIdType        viaVertexId() const;

    /**
     * @return  The Edge id of the 'to' Edge.
     */
    EdgeIdType          toEdgeId() const;

// INQUIRY
    /** Check if an Edge is in this restriction.
     * @param   Edge Id to check.
     * @return  true if the edge is part of this restriction, false if not.
     */
    bool                isInRestriction(EdgeIdType edgeId) const;

    /** Check if Travel from 'from' to 'to' is restricted.
     * @param   fromEdgeId    Travel from edge.
     * @param   toEdgeId      Travel to edge.
     * @return  true if travle is restricted.
     */
    bool                isRestricted(
                            EdgeIdType fromEdgeId,
                            EdgeIdType toEdgeId) const;

protected:
private:
    TurningRestrictionType                mType {NONE};
    EdgeIdType                            mFromEdgeId;
    TurningViaType                        mViaType {VIA_NODE};
    std::string                           mViaOsmIds;
    VertexIdType                          mViaVertexId;
    EdgeIdType                            mToEdgeId;
    static std::vector<std::string>       sTypeStrings;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMTURNINGRESTRICTION_H_ */
