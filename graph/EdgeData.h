/**  Abstract base class for auxilliary data connected to Edges.
 *
 * #include "EdgeData.h"
 *
 * The data relating to an edge differs if the Edge is an original Graph edge
 * or if it is a Line Graph edge. By using different EdgeData one can use the
 * same Topology class for both types of graphs.
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_EDGEDATA_H_
#define GRAPH_EDGEDATA_H_

// SYSTEM INCLUDES
//
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//


class EdgeData
{
public:
// LIFECYCLE

   /** Default constructor.
    */
   EdgeData() {};


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   EdgeData(const EdgeData& from) = default;


   /** Destructor.
    */
   virtual ~EdgeData() = default;


// OPERATORS
   friend
   std::ostream&    operator<<(std::ostream& os, const EdgeData& rEdgeData)
   {
       rEdgeData.print(os);
       return os;
   }

// OPERATIONS
// ACCESS
// INQUIRY

protected:
   virtual void     print(std::ostream& os) const = 0;
private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_EDGEDATA_H_ */
