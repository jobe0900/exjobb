/**  Abstract base class for data connected to Vertices.
 *
 * #include "EdgeData.h"
 *
 * The data relating to a Vertex differs if the Vertex is an original Graph
 * Vertex or if it is a Line Graph Node. By using different VertexData one
 * can use the same Topology class for both types of graphs.
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_VERTEXDATA_H_
#define GRAPH_VERTEXDATA_H_

// SYSTEM INCLUDES
//
#include <ostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//


class VertexData
{
public:
// LIFECYCLE

   /** Default constructor.
    */
   VertexData() {};


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   VertexData(const VertexData& from) = default;


   /** Destructor.
    */
   virtual ~VertexData() = default;


// OPERATORS
   friend
   std::ostream&    operator<<(std::ostream& os, const VertexData& rVertexData)
   {
       rVertexData.print(os);
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





#endif /* GRAPH_VERTEXDATA_H_ */
