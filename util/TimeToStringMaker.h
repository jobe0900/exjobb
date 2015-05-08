/**  Abstract Base class to provide strings based on time.
 *
 * #include "TimeToStringMaker.h"
 *
 * @author Jonas Bergman
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
 * An interface for classes who provide strings from times.
 */
class TimeToStringMaker
{
public:
	// LIFECYCLE

	/** Default constructor.
	 */
	TimeToStringMaker() {};

	/** Destructor.
	 */
	virtual ~TimeToStringMaker() {};

	// OPERATORS
	// OPERATIONS

	/** Get the current time as a string.
	 *
	 * @return	A string representation of the time.
	 */
	virtual	std::string		getCurrentTimeString() = 0;

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
