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
#include "../graph/Topology.h"
#include "../config/Configuration.h"
#include "../graph/Edge.h"
#include "../graph/Vertex.h"

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
        : mBuildTempTopology(false), mrConfig(rConfig)
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
//    /** Fill the topology with data from the MapProvider.
//     * @param   rTopology   The Topology to fill with data.
//     * @throws  MapProviderException, TopologyException
//     */
    virtual void    getTopology(Topology& rTopology) = 0;

    /** Fill the topology with data from the MapProvider,
     * and mark the edges in the topology with restrictions and costs.
     * @param   rTopology   The Topology to fill with data.
     * @throws  MapProviderException, TopologyException
     */
//    virtual void    getMapData(Topology& rTopology) = 0;

    /** Read tags that might impose restrictions and costs and add them to
     * the edges in the topology.
     * @param   rTopology       The Topology with edges to get updated.
     * @throws  MapProviderException, RestrictionsException
     */
    virtual void    setRestrictionsAndCosts(
        Topology&      rTopology) = 0;

//    /** Fill the Restrictions with restrictions for edges and vertices.
//     * Might also update Edges in the topology.
//     * @param   rRestrictions   The Restrictions to fill with data.
//     * @param   rTopology       The Topology that might get updated.
//     * @throws  MapProviderException, RestrictionsException
//     */
//    virtual void    getRestrictions(
//        Restrictions&  rRestrictions,
//        Topology&      rTopology) = 0;
// ACCESS
// INQUIRY

protected:
    bool                 mBuildTempTopology;
    const Configuration& mrConfig;
private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_MAPPROVIDER_H_ */
