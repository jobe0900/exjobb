/**  A helper class for building and transforming Graphs.
 *
 * #include "GraphTransformer.h" <BR>
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_GRAPHTRANSFORMER_H_
#define GRAPH_GRAPHTRANSFORMER_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Graph.h"
#include "GraphException.h"
#include "../mapprovider/MapProvider.h"

// FORWARD REFERENCES
//


class GraphTransformer
{
public:
// LIFECYCLE

   /** Default constructor.
    */
   GraphTransformer(void) = delete;


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   GraphTransformer(const GraphTransformer& from) = delete;


   /** Destructor.
    */
   ~GraphTransformer(void) = delete;


// OPERATORS
   /** Create a LineGraph from the original.
    * This is done by iterating the edges and making connections to the
    * outgoing edges from the edge's target vertex.
    * @param    pGraph    Pointer to the original Graph.
    * @return   pointer to LineGraph.
    * @throws
    */
   static Graph*  toLineGraph(const Graph* pGraph);


// OPERATIONS
// ACCESS
// INQUIRY

protected:
private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_GRAPHTRANSFORMER_H_ */
