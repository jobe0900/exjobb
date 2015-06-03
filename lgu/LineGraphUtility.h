/**  A helper class for building and transforming Graphs.
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

// LOCAL INCLUDES
//
#include "LineGraphUtilityException.h"
#include "../config/ConfigurationReader.h"
#include "../graph/Graph.h"
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
    Graph::LineGraphType*  getLineGraph();

// ACCESS
// INQUIRY

protected:
private:
// HELPERS
    void    init();
    void    initConfiguration();
    void    initMapProvider();
    void    initTopology();
    void    buildGraph();
//    void    transformGraph();

// ATTRIBUTES
    const std::string&  mrSettingsfile;
    Configuration       mConfig;
    MapProvider*        mpMapProvider;
    Topology            mTopology;
    Graph*              mpGraph;

// CONSTANTS
//    static constexpr const char* SETTINGSFILE = "settings.json";
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* LGU_LINEGRAPHUTILITY_H_ */
