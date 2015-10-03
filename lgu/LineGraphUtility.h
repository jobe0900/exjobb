/**  The class to call to request a linegraph for routing.
 *
 * #include "LineGraphUtility.h"
 *
 * @author Jonas Bergman
 */

#ifndef LGU_LINEGRAPHUTILITY_H_
#define LGU_LINEGRAPHUTILITY_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//
#include <boost/graph/copy.hpp>

// LOCAL INCLUDES
//
#include "LineGraphUtilityException.h"
#include "../config/ConfigurationReader.h"
#include "../graph/GraphBuilder.h"
#include "../mapprovider/MapProvider.h"

// FORWARD REFERENCES
//


/** A class to run the fetching of data from database, through to complete
 * weighted linegraph.
 */
class LineGraphUtility
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    LineGraphUtility() = delete;

    /** Constructor.
     * @param   rFilename   The path to the configuration file.
     */
    LineGraphUtility(const std::string& rFilename);

    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    LineGraphUtility(const LineGraphUtility& from) = delete;


    /** Destructor.
     */
    ~LineGraphUtility(void);


// OPERATORS
// OPERATIONS
    /** Return a LineGraph
     */
    LineGraphType*   getLineGraph();

    /** Re-read the topology if there has been a change in the database.
     */
    void             updateTopology();

    /** Re-apply restrictions and costs on the topology fi there has been changes.
     */
    void             updateRestrictionsAndCosts();

    /** Save the LineGraph to storage.
     * This is a hack to be able to demo the line graph in PostGis and JOSM.
     */
    void             persistLineGraph();

// ACCESS
// INQUIRY

protected:
private:
// HELPERS
    void    init();
    void    initConfiguration();
    void    initMapProvider();
    void    initTopology();
    void    initRestrictionsAndCosts();
    void    buildGraph();

// ATTRIBUTES
    const std::string&  mrSettingsfile;
    Configuration       mConfig;
    MapProvider*        mpMapProvider;
    Topology            mTopology;
    GraphBuilder*       mpGraph;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* LGU_LINEGRAPHUTILITY_H_ */
