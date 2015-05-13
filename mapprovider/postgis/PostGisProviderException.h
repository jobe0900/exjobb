/**  Exception thrown by the Database package.
 *
 * #include "PostGisProviderException.h"
 *
 * A longer description.
 *
 * @see something
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_POSTGIS_POSTGISPROVIDEREXCEPTION_H_
#define MAPPROVIDER_POSTGIS_POSTGISPROVIDEREXCEPTION_H_

// SYSTEM INCLUDES
//
#include <exception>
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//

// FORWARD REFERENCES
//

/**
 * Exception to throw from the 'database' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class PostGisProviderException : public std::exception
{
public:
// LIFECYCLE

	/** Default constructor.
	 */
	PostGisProviderException() = delete;

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	PostGisProviderException(const std::string& rMessage) noexcept
		: std::exception(), mMessage(rMessage)
	{}

// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
	const char* what() const noexcept
	{return (mMessage + " " + std::exception::what()).c_str();}

protected:
private:
// ATTRIBUTES
	std::string		mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_POSTGIS_POSTGISPROVIDEREXCEPTION_H_ */
