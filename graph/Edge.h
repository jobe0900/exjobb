/**  Data structure for edges in Topology.
 *
 * #include "Edge.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_EDGE_H_
#define GRAPH_EDGE_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Vertex.h"
#include "EdgeData.h"

// FORWARD REFERENCES
//
typedef unsigned        EdgeIdType;

/**
 * Data structure for edges in the topology.
 */
class Edge
{
public:

// LIFECYCLE
	/** Constructor.
	 * @param	id		Id for this Edge
	 * @param	source	Source vertex
	 * @param	target	Target vertex
	 */
	Edge(EdgeIdType   id,
	     VertexIdType source,
	     VertexIdType target);
//	     Direction    direction = BOTH);

	/** Copy constructor.
	 * @param   from    The Edge to make a copy of.
	 */
	Edge(const Edge& from);

	~Edge();

// OPERATORS
	/** Textual output of Edge.
	 */
	friend
	std::ostream&	operator<<(std::ostream& os, const Edge& rEdge);

	/** Compare it two edges share the same id, sourc and target.
	 * @param   rhs     The edge to compare to.
	 * @return  True if they are equal, false if not.
	 */
	bool			operator==(const Edge& rhs) const;

// OPERATIONS
	/** Set the EdgeData of this Edge.
	 * @param   pEdgeData   Pointer to EdgeData.
	 */
	void            setEdgeData(EdgeData* pEdgeData);

// ACCESSORS
	/**
	 * @return  The id of this edge.
	 */
	EdgeIdType		id()        const;

	/**
	 * @return  The source vertex for this edge.
	 */
	VertexIdType	source()    const;

	/**
	 * @return  The target vertex of this edge.
	 */
	VertexIdType	target()    const;

	/**
	 * @return  Pointer to the EdgeData for this edge, or 'nullptr' if none.
	 */
	EdgeData*       edgeData()  const;

// INQUIRY

private:
// ATTRIBUTES
	EdgeIdType		mId;
	VertexIdType	mSource;
	VertexIdType	mTarget;
	EdgeData*       mpEdgeData;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGE_H_ */
