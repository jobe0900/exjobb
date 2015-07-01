/**  Data structure for configuration of costs.
 *
 * #include "CostConfig.h"
 *
 * @author Jonas Bergman
 */

#ifndef CONFIG_COSTCONFIG_H_
#define CONFIG_COSTCONFIG_H_

// SYSTEM INCLUDES
//
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//

/**
 * Information about names in the database for topology data.
 */
struct CostConfig
{
// TYPES
    struct DefaultSpeed
    {
        struct HighLowSpeed
        {
            int high {0};
            int low {0};
        };

        HighLowSpeed motorway;
        HighLowSpeed motorway_link;
        HighLowSpeed trunk;
        HighLowSpeed trunk_link;
        HighLowSpeed primary;
        HighLowSpeed primary_link;
        HighLowSpeed secondary;
        HighLowSpeed secondary_link;
        HighLowSpeed tertiary;
        HighLowSpeed tertiary_link;
        HighLowSpeed unclassified;
        HighLowSpeed residential;
        HighLowSpeed living_street;
    };

// ATTRIBUTES
    DefaultSpeed defaultSpeed;

// CONSTANTS
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_COSTCONFIG_H_ */
