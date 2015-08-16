/**  Read in sample topology from a json file.
 *
 * #include "JsonTestProvider.h"
 *
 * @author Jonas Bergman
 */
#ifndef MAPPROVIDER_JSONTEST_JSONTESTPROVIDER_H_
#define MAPPROVIDER_JSONTEST_JSONTESTPROVIDER_H_

// SYSTEM INCLUDES
//

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../MapProvider.h"

// FORWARD REFERENCES
//

/**
 * Interface for getting map data from file or database.
 */
class JsonTestProvider : public MapProvider
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    JsonTestProvider() = delete;

    /** Constructor.
     * Construct a MapProvider based on the configurations given.
     */
    JsonTestProvider(const Configuration& rConfig);

    /** Copy constructor.
     *
     * @param from The value to copy to this object.
     */
    JsonTestProvider(const JsonTestProvider& from) = delete;


    /** Destructor.
     */
    virtual ~JsonTestProvider(void);


// OPERATORS
// OPERATIONS
    virtual void    getTopology(Topology& rTopology);

    virtual void    setRestrictionsAndCosts(Topology& rTopology);

// ACCESS
// INQUIRY

protected:
private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_JSONTEST_JSONTESTPROVIDER_H_ */
