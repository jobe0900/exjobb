/**  Class to provide string based on ms since epoch.
 *
 * #include "EpochMsTimeToString.h"
 *
 * @author Jonas Bergman
 */

#ifndef UTIL_EPOCHMSTIMETOSTRING_H_
#define UTIL_EPOCHMSTIMETOSTRING_H_

// SYSTEM INCLUDES
//
#include <string>
#include <chrono>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "TimeToStringMaker.h"

// FORWARD REFERENCES
//

/**
 * A class to statically provide strings from times in a format like
 * '1431110306615', which is the count of ms since epoch.
 */
class EpochMsTimeToString : public TimeToStringMaker
{
public:
// LIFECYCLE

	/** Default constructor.
	 */
	EpochMsTimeToString();

	/** Virtual destructor needed.
	 */
	virtual ~EpochMsTimeToString();

// OPERATORS
// OPERATIONS

	/** Get the current time as a string like '1431110306615'.
	 *
	 * @return	A string representation of the time.
	 */
	virtual	std::string		getCurrentTimeString();
// ACCESS
// INQUIRY

protected:

private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* UTIL_EPOCHMSTIMETOSTRING_H_ */
