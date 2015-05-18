/**  A helper class for building and transforming Graphs.
 *
 * #include "GraphBuilder.h" <BR>
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_GRAPHBUILDER_H_
#define GRAPH_GRAPHBUILDER_H_

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


class GraphBuilder
{
public:
// LIFECYCLE

   /** Default constructor.
    */
   GraphBuilder(void) = delete;


   /** Copy constructor.
    *
    * @param from The value to copy to this object.
    */
   GraphBuilder(const GraphBuilder& from) = delete;


   /** Destructor.
    */
   ~GraphBuilder(void) = delete;


// OPERATORS
   /** Construct a Graph from topology given by the MapProvider.
    * @param    rMapProvider    Reference to the MapProvider to use.
    * @return   pointer to Graph.
    * @throws MapProviderException, TopologyException, GraphException
    */
   static Graph*  buildGraph(const MapProvider& rMapProvider);


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

#endif /* GRAPH_GRAPHBUILDER_H_ */
