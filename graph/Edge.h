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
#include <limits>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Vertex.h"
//#include "EdgeData.h"

// FORWARD REFERENCES
//
typedef unsigned            EdgeIdType;
typedef unsigned long int   OsmIdType;

/**
 * Data structure for edges in the topology.
 */
class Edge
{
public:
// TYPES
    //------------------------------------------------------------------------
    enum DirectionType
    {
        BOTH,       // bidirectional
        TO_FROM,    // one-way: from Source to Target
        FROM_TO     // one-way: from Target to Source
    };

    //------------------------------------------------------------------------
    /** A data structure for geometric information for the Edge.
     * Bearing is the compass direction in degrees at source and at target.
     */
    struct GeomData
    {
        double      length {1.0};
        Point       centerPoint;
        int         sourceBearing {0};
        int         targetBearing {0};

        /** Constructor. */
        GeomData() = default;

        /** Constructor. */
        GeomData(double length,
                 Point  centerPoint,
                 int    sourceBearing,
                 int    targetBearing);
    };

    //------------------------------------------------------------------------
    /** A data structure for road related information for the Edge.
     */
    struct RoadData
    {
        DirectionType   direction {BOTH};
        size_t          nrLanes {1};

        /** Constructor. */
        RoadData() = default;

        /** Constructor. */
        RoadData(DirectionType direction, size_t nrLanes);

        /** Print this information. */
        void print(std::ostream& os) const;
    };

// LIFECYCLE
    /** Constructor.
     */
    Edge() = delete;

    /** Constructor.
     * @param	id		    Id for this Edge
     * @param   osmId       The original OsmId this edge belongs to.
     * @param	source	    Source vertex
     * @param	target	    Target vertex
     * @param   geomData    Geometric data for the edge.
     * @param   roadData    Road data for the edge.
     */
    Edge(EdgeIdType      id,
         OsmIdType       osmId,
         VertexIdType    source,
         VertexIdType    target,
         GeomData        geomData,
         RoadData        roadData);

    /** Constructor.
     * Using default values for geometry and road.
     * @param	id		Id for this Edge
     * @param   osmId   The original OsmId this edge belongs to.
     * @param	source	Source vertex
     * @param	target	Target vertex
     */
    Edge(EdgeIdType      id,
         OsmIdType       osmId,
         VertexIdType    source,
         VertexIdType    target);

    /** Copy constructor.
     * @param   from    The Edge to make a copy of.
     */
    Edge(const Edge& from) = default;

    /** Destructor.
     */
    ~Edge() = default;

// OPERATORS
    /** Textual output of Edge.
     */
    friend
    std::ostream&	operator<<(std::ostream& os, const Edge& rEdge);

// OPERATIONS
    /** Set the Geometric data for this edge.
     * @param   geomData    The GeomData to use.
     */
    void              setGeomData(GeomData geomData);

    /** Set the Road data for this edge.
     * @param   roadData    The RaodData to use.
     */
    void              setRoadData(RoadData geomData);

    void              setOsmId(OsmIdType osmId);

// ACCESSORS
    /**
     * @return  The id of this edge.
     */
    EdgeIdType		  id()        const;

    /**
     * @return  The source vertex for this edge.
     */
    VertexIdType	  source()    const;

    /**
     * @return  The target vertex of this edge.
     */
    VertexIdType	  target()    const;

    /**
     * @return  The original OSM id for this edge.
     */
    OsmIdType         osmId()     const;

    /**
     * @return The geometric data for this edge.
     */
    const GeomData&   geomData()  const;

    /**
     * @return The road data for this edge.
     */
    const RoadData&   roadData()  const;

// INQUIRY

private:
// ATTRIBUTES
    EdgeIdType		mId;
    OsmIdType       mOsmId;
    VertexIdType	mSource;
    VertexIdType	mTarget;
    GeomData        mGeomData;
    RoadData        mRoadData;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGE_H_ */
