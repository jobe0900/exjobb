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
    std::string     providerName;

    std::string     tempTopoName;
    std::string     topoName;

    std::string     roadsPrefix;
    std::string     topologySchemaPrefix;

    std::string     edgeTableName;
    std::string     edgeIdColumnName;
    std::string     sourceColumnName;
    std::string     targetColumnName;
    std::string     edgeGeomColumnName;

    std::string     vertexTableName;
    std::string     vertexIdColumnName;
    std::string     vertexGeomColumnName;

    std::string     testFile;

// CONSTANTS
    static constexpr const char* PROVIDER_POSTGIS = "postgis";
    static constexpr const char* PROVIDER_PGROUTING = "pgrouting";
    static constexpr const char* PROVIDER_JSONTEST = "jsontest";
    static constexpr const char* TEMP_TOPO_NAMEBASE = "epoch_ms";


};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_TOPOLOGYCONFIG_H_ */
