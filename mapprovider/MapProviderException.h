/**  Exception thrown by the MapProvider package.
 *
 * #include "MapProviderException.h"
 *
 * @author Jonas Bergman
 */

#ifndef MAPPROVIDER_MAPPROVIDEREXCEPTION_H_
#define MAPPROVIDER_MAPPROVIDEREXCEPTION_H_

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
 * Exception to throw from the 'MapProvider' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class MapProviderException : public std::exception
{
public:
// LIFECYCLE

	/** Default constructor.
	 */
	MapProviderException() = delete;

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	MapProviderException(const std::string& rMessage) noexcept
		: std::exception(), mMessage(rMessage)
	{}

// OPERATORS
// OPERATIONS
// ACCESS
// INQUIRY
	const char* what() const noexcept
	{return mMessage.c_str();}

protected:
private:
// ATTRIBUTES
	std::string		mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* MAPPROVIDER_MAPPROVIDEREXCEPTION_H_ */
