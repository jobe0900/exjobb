/** Data connected to Edges (Lines) in the LineGraph topology.
 *
 * #include "LineGraphEdgeData.h"
 *
 * The data structure for a LineGraph Line (edge) is simple, it is just the
 * weight (cost) for traveling that line. The source Node (vertex) of this
 * Line is really the incoming edge at a vertex, and the target Node is a
 * connected outgoing line from that vertex.
 *
 * The cost of the Line is the cost of travel along the incoming edge +
 * the turn cost at the vertex.
 *
 * This structure also keeps track of the original Vertex id that is the
 * connection between the source and target Nodes (originally Edges).
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_LINEGRAPHEDGEDATA_H_
#define GRAPH_LINEGRAPHEDGEDATA_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "EdgeData.h"

// FORWARD REFERENCES
//


class LineGraphEdgeData : public EdgeData
{
public:
// TYPES

// LIFECYCLE

   /** Default constructor.
    */
   LineGraphEdgeData();


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   LineGraphEdgeData(const LineGraphEdgeData& from) = default;


   /** Destructor.
    */
   virtual ~LineGraphEdgeData();


// OPERATORS
// OPERATIONS
   /** Set the cost for traveling this Line.
    * @param    cost    The cost;
    */
   void             setCost(double cost);

   /** Set the id of the vertex this Line corresponds to.
    * @param    vertexId    The Id of the vertex.
    */
   void             setViaVertexId(VertexIdType vertexId);

// ACCESS
   /**
    * @return   Cost for traveling this line.
    */
   double           cost()      const;

   /**
    * @return   Id of the corresponding vertex.
    */
   VertexIdType     viaVertexId() const;

// INQUIRY

protected:
   virtual void     print(std::ostream& os) const;

private:
   double           mCost;
   VertexIdType     mViaVertexId;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_LINEGRAPHEDGEDATA_H_ */
