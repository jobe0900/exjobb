/*
 * Restriction.h
 *
 *  Created on: 27 maj 2015
 *      Author: jonas
 */


/**  The Restrictions class is a container for
 *   EdgeRestrictions and VertexRestrictions.
 *
 * #include "Restrictions.h" <BR>
 *
 * @author  Jonas Bergman
 */

#ifndef GRAPH_RESTRICTION_H_
#define GRAPH_RESTRICTION_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "EdgeRestrictions.h"
#include "RestrictionsException.h"
#include "Edge.h"
#include "Vertex.h"

// FORWARD REFERENCES
//

/**
 * A container for EdgeRestrictions and VertexRestrictions.
 */
class Restrictions
{
public:
// TYPES
// LIFECYCLE

    /** Default constructor.
     */
    Restrictions() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    Restrictions(const Restrictions& from) = default;


    /** Destructor.
     */
    ~Restrictions() = default;


// OPERATORS
// OPERATIONS
// ACCESS
    const EdgeRestrictions&     edgeRestrictions() const
                                { return mEdgeRestrictions; }

    EdgeRestrictions&           edgeRestrictions()
                                { return mEdgeRestrictions; }

//    const VertexRestrictions&   vertexRestrictions() const
//                                { return mVertexRestrictions; }
//
//    VertexRestrictions&         vertexRestrictions()
//                                { return mVertexRestrictions; }

// INQUIRY
protected:
private:
    EdgeRestrictions    mEdgeRestrictions {};
    //VertexRestrictions  mVertexRestrictions;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_RESTRICTION_H_ */
