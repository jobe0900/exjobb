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

// ACCESS
// INQUIRY

protected:
private:
// ATTRIBUTES
    DatabaseConfig mDbConfig;
    TopologyConfig mTopoConfig;
    VehicleConfig  mVehicleConfig;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* CONFIG_CONFIGURATION_H_ */
