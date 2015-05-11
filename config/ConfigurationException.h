/**  Exception thrown by the Configuration package.
 *
 * #include "ConfigurationException.h"
 *
 * A longer description.
 *
 * @see something
 * @author Jonas Bergman
 */

#ifndef CONFIG_CONFIGURATIONEXCEPTION_H_
#define CONFIG_CONFIGURATIONEXCEPTION_H_

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
 * Exception to throw from the 'config' package.
 * More information of the type of exception is given in the 'what()' message.
 */
class ConfigurationException : public std::exception
{
public:
// LIFECYCLE

	/** Constructor taking a message to display.
	 *
	 * @param	message		The message to prepend when 'what()' is called.
	 */
	ConfigurationException(const std::string& rMessage) noexcept
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
	ConfigurationException();

// ATTRIBUTES
	std::string		mMessage;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_CONFIGURATIONEXCEPTION_H_ */
