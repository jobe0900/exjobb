/**  Exception thrown by the Database package.
 *
 * #include "DatabaseException.h"
 *
 * A longer description.
 *
 * @see something
 * @author Jonas Bergman
 */

#ifndef OSM_PROVIDER_OSMPROVIDEREXCEPTION_H_
#define OSM_PROVIDER_OSMPROVIDEREXCEPTION_H_

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
class OsmProviderException : public std::exception
{
public:
	// LIFECYCLE

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	OsmProviderException(const std::string& rMessage) noexcept
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
	/** Default constructor.
	 * Hidden.
	 */
	OsmProviderException();

	// ATTRIBUTES
	std::string		mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_PROVIDER_OSMPROVIDEREXCEPTION_H_ */
