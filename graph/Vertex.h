/**  Data structure for vertices in Topology.
 *
 * #include "Vertex.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_VERTEX_H_
#define GRAPH_VERTEX_H_

// SYSTEM INCLUDES
//
#include <limits>
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../util/Point.h"

// FORWARD REFERENCES
//
typedef long        VertexIdType;

/**
 * Data structure for vertices in the topology.
 */
class Vertex
{
public:
// TYPES and CONSTANTS
    static const VertexIdType MAX_ID;
// LIFECYCLE
	/** Constructor.
	 * @param	id		         Id for this vertex.
	 * @param	point	         The Point (geometry).
	 */
	Vertex(VertexIdType id, Point point);

	/** Default constructor. Deleted */
	Vertex() = delete;

	/** Copy constructor. Default. */
	Vertex(const Vertex&) = default;

// OPERATORS
	friend
	std::ostream&	operator<<(std::ostream& os, const Vertex& rVertex);

	bool			operator==(const Vertex& rhs) const;

// OPERATIONS
// ACCESS
	/**
	 * @return  The id of this Vertex.
	 */
	VertexIdType	id() const;

	/**
	 * @return  The coordinates for this Vertex.
	 */
	Point		    point() const;

//INQUIRY
	/**
	 * @return  True if the Vertex has restrictions.
	 */
	bool            hasRestrictions() const;

private:
// ATTRIBUTES
	VertexIdType	mId;
	Point		    mPoint;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_VERTEX_H_ */
