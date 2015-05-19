/*
 * TopoEdgeData.h
 *
 *  Created on: 19 maj 2015
 *      Author: jonas
 */


/** Data connected to Edges in the original topology.
 *
 * #include "TopoEdgeData.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TOPOEDGEDATA_H_
#define GRAPH_TOPOEDGEDATA_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "EdgeData.h"

// FORWARD REFERENCES
//


class TopoEdgeData : public EdgeData
{
public:
// TYPES
    enum Direction
    {
        BOTH,       // bidirectional
        TO_FROM,    // one-way: from Source to Target
        FROM_TO     // one-way: from Target to Source
    };

// LIFECYCLE

   /** Default constructor.
    */
   TopoEdgeData();


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   TopoEdgeData(const TopoEdgeData& from) = default;


   /** Destructor.
    */
   virtual ~TopoEdgeData();


// OPERATORS
// OPERATIONS
   void             setDirection(Direction direction);
   void             setNrLanes(size_t nrLanes);
   void             setLength(double length);
// ACCESS
   Direction        direction() const;
   size_t           nrLanes()   const;
   double           length()    const;
// INQUIRY

protected:
   virtual void     print(std::ostream& os) const;

private:
   Direction    mDirection;
   size_t       mNrLanes;
   double       mLength;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TOPOEDGEDATA_H_ */
