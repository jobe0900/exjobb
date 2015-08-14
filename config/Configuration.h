/**  A container for configurations.
 *
 * #include "Configuration.h"
 *
 * @author Jonas Bergman
 */

#ifndef CONFIG_CONFIGURATION_H_
#define CONFIG_CONFIGURATION_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "DatabaseConfig.h"
#include "VehicleConfig.h"
#include "CostConfig.h"
#include "../osm/OsmAccess.h"
#include "../osm/OsmBarrier.h"

// FORWARD REFERENCES
//

/**
 * This class holds configurations for different parts of the utility.
 * The ConfigurationReader is friend so it can populate the different
 * configurations.
 */
class Configuration
{
    friend class ConfigurationReader;
public:
// LIFECYCLE

    /** Default constructor.
     */
    Configuration() = default;


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    Configuration(const Configuration& from) = delete;


    /** Destructor.
     */
    ~Configuration(void) = default;


// OPERATORS
// OPERATIONS
// ACCESS
    /** Get the database related parts of the configuration.
     * @return  Reference to a DatabaseConfig.
     */
    const DatabaseConfig& getDatabaseConfig() const;

    /** Get the topology related parts of the configuration.
     * @return  Reference to a TopologyConfig.
     */
    const TopologyConfig& getTopologyConfig() const;

    /** Get the vehicle related parts of the configuration.
     * @return  Reference to a VehicleConfig.
     */
    const VehicleConfig&  getVehicleConfig() const;

    /** Get the rules for which values of the `access`-tag allows access
     * and hence which values restricts access to an Edge.
     * @return  Reference to an AccessRule
     */
    const OsmAccess::AccessRule&
                          getAccessRule() const;

    /** Get the rules for which values of the `barrier`-tag restricts access
     * @return  Reference to an RestrictionsRule
     */
    const OsmBarrier::RestrictionsRule&
                          getBarrierRestrictionsRule() const;

    /** Get the rules for which values of the `barrier`-tag costs to pass
     * @return  Reference to an CostsRule
     */
    const OsmBarrier::CostsRule&
                          getBarrierCostsRule() const;

    /** Get the cost related parts of the configuration.
     * @return  Reference to a CostConfig.
     */
    const CostConfig&     getCostConfig() const;

// INQUIRY

protected:
private:
// ATTRIBUTES
    DatabaseConfig          mDbConfig;
    TopologyConfig          mTopoConfig;
    VehicleConfig           mVehicleConfig;
    CostConfig              mCostConfig;
    OsmAccess::AccessRule   mAccessRule;
    OsmBarrier::CostsRule   mBarrierCostsRule;
    OsmBarrier::RestrictionsRule mBarrierRestrictionsRule;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* CONFIG_CONFIGURATION_H_ */
