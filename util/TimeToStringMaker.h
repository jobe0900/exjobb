/**  Static class to provide strings based on time.
 *
 * #include "TimeToStringMaker.h"
 *
 * @author  Jonas Bergman
 */

#ifndef UTIL_TIMETOSTRINGMAKER_H_
#define UTIL_TIMETOSTRINGMAKER_H_

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
 * Class who provide strings from times.
 */
class TimeToStringMaker
{
public:
// LIFECYCLE

    /** Default constructor.
     */
    TimeToStringMaker() = delete;

    /** Copy constructor */
    TimeToStringMaker(const TimeToStringMaker& from) = delete;

// OPERATORS
// OPERATIONS

    /** Get the current time as a string.
     *
     * @return    A string representation of the time.
     */
    static std::string        getEpochMsTimeString();

// ACCESS
// INQUIRY

protected:

private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* UTIL_TIMETOSTRINGMAKER_H_ */
