/**  Data structure for configuration of topology data in database.
 *
 * #include "TopologyConfig.h"
 *
 * @author Jonas Bergman
 */

#ifndef CONFIG_TOPOLOGYCONFIG_H_
#define CONFIG_TOPOLOGYCONFIG_H_

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
struct TopologyConfig
{
// ATTRIBUTES
    std::string     roadsTableName;
    std::string     topologySchemaName;

    std::string     edgeTableName;
    std::string     edgeIdColumnName;
    std::string     sourceColumnName;
    std::string     targetColumnName;
    std::string     edgeGeomColumnName;

    std::string     vertexTableName;
    std::string     vertexIdColumnName;
    std::string     vertexGeomColumnName;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_TOPOLOGYCONFIG_H_ */
