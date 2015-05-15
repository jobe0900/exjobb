/**  Abstract base class giving the interface for different sources of
 * topology map data.
 *
 * #include "MapProvider.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_MAPPROVIDER_H_
#define MAPPROVIDER_MAPPROVIDER_H_

// SYSTEM INCLUDES
//
#include <map>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../config/Configuration.h"
#include "../graph/TopologyEdge.h"
#include "../graph/TopologyTypes.h"
#include "../graph/TopologyVertex.h"

// FORWARD REFERENCES
//

/**
 * Interface for getting map data from file or database.
 */
class MapProvider
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    MapProvider() = delete;

    /** Constructor.
     * Construct a MapProvider based on the configurations given.
     */
    MapProvider(const Configuration& rConfig)
        : mrConfig(rConfig), mBuildTempTopology(false)
    {}

    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    MapProvider(const MapProvider& from) = delete;


    /** Destructor.
     */
    virtual ~MapProvider(void) {}


// OPERATORS
// OPERATIONS
    /** Get edges from the provider of map data.
     * @param   rEdgeMap    Reference to a map with edge id => edge.
     */
    virtual void    getTopologyEdges(
                        std::map<EdgeId, TopologyEdge>& rEdgeMap) = 0;

    /** Get vertices from the provider of map data.
     * @param   rVertexMap    Reference to a map with vertex id => vertex.
     */
    virtual void    getTopologyVertices(
                        std::map<VertexId, TopologyVertex>& rVertexMap) = 0;
// ACCESS
// INQUIRY

protected:
private:
    const Configuration& mrConfig;
    const bool           mBuildTempTopology;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_MAPPROVIDER_H_ */
