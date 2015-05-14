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
 * It stores them in pointer, and is responsible for deleting them.
 */
class Configuration
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    Configuration();


    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    Configuration(const Configuration& from) = delete;


    /** Destructor.
     */
    ~Configuration(void);


// OPERATORS
// OPERATIONS
    /** Get the database related parts of the configuration.
     * @return  Pointer to a DatabaseConfig.
     */
    const DatabaseConfig* getDatabaseConfig() const;

    /** Get the vehicle related parts of the configuration.
     * @return  Pointer to a VehicleConfig.
     */
    const VehicleConfig*  getVehicleConfig() const;

    /** Set the DatabaseConfig.
     * @param   pDbConfig   Pointer to a database configuration
     */
    void                  setDatabaseConfig(const DatabaseConfig* pDbConfig);

    /** Set the VehicleConfig.
     * @param   pVehicleConfig   Pointer to a vehicle configuration
     */
    void                  setVehicleConfig(const VehicleConfig* pVehicleConfig);

// ACCESS
// INQUIRY

protected:
private:
// ATTRIBUTES
    const DatabaseConfig* mpDbConfig;
    const VehicleConfig*  mpVehicleConfig;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//



#endif /* CONFIG_CONFIGURATION_H_ */
