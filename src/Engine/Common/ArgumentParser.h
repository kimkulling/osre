/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "Common/osre_common.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Common {

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	Helper class to handle incoming arguments.
///
///	The list of supported arguments must be separated with a : .
///	You can also specify a number of expected values for a specific argument with the following syntax: 
///	@code
///	arg<numParameter>
///	@endcode
//--------------------------------------------------------------------------------------------------------------------
class OSRE_EXPORT ArgumentParser {
public:
    ///	@brief Stores a single argument description.
    struct Argument {
        String mArgument;	///< The argument as a string.
        String mDesc;       ///< The description for the argument ( used for help texts ).
        ui32 mNumArgs;		///< The number of expected arguments for it.

        /// @brief The default class constructor.
        Argument();

        /// @brief The class constructor with all parameters.
        /// @param arg      The argument.
        /// @param desc     The argument description    
        /// @param numArgs  The number of expected arguments.
        Argument( const String &arg, const String &desc, ui32 numArgs );
    };

    ///	@brief	The class constructor with arguments and the requested argument support.
    ///	@param	argc            [in] The number of incoming arguments.
    ///	@param	ppArgv          [in] The arguments itself.
    ///	@param	supportedArgs   [in] Description of the supported arguments, separated by :.
    ///	@param  desc            [in] The argument description, separated by :.
    ArgumentParser( i32 argc, const c8 *ppArgv[], const String &supportedArgs, const String &desc );
    
    ///	@brief	The class destructor.
    ~ArgumentParser() = default;

    /// @brief Will add a new error string.
    /// @param[in] error    The new error.
    void addError(const String &error);

    /// @brief Will return all error strings.
    /// @return The error strings.
    const String &getLastErrors() const;

    ///	@brief	Returns true, if any arguments are supported
    ///	@return	true, if arguments are supported.
    bool hasSupportedArguments() const;

    /// @brief  Will show the help.
    /// @return The help string;
    String showHelp() const;

    ///	@brief	Returns next argument and its content.
    ///	@param	arg     	[in] Next argument.
    ///	@param	value		[in] Content of argument.
    ///	@return	true, if another arguments are supported.
    bool getNext( String &arg, String &value );

    ///	@brief	Restarts the iteration. If no iteration is active nothing happens.
    void reset();

    ///	@brief	The argument parser will be cleared.
    void clear();

    ///	@brief	Returns true, if argument is supported.
    ///	@param	arg         [in] Argument to check for support.
    ///	@return	true, if argument is supported.
    bool isSupported( const String &arg ) const;

    ///	@brief	Returns the number of expected values from a given argument.
    ///	@param	argument	[in] The argument in the following form arg<numParam>.
    ///	@return	The number of expected parameters.
    ui32 getNumValues( const String &argument ) const;

    ///	@brief	Returns argument content.
    ///	@param	arg     	[in] Requested argument.
    ///	@return	Content of argument.
    const String &getArgument( const String &arg ) const;

    ///	@brief	Returns true, if requested argument is part of the managed argument list.
    ///	@param	arg     	[in] Argument name.
    ///	@return	True, if argument was detected in managed argument list. False if not.
    bool hasArgument( const String &arg ) const;

    ///	@brief	Will validate the detected arguments from the command line.
    ///	@return	true, if the arguments are all valid, false if not.
    bool hasValidArgs() const;

    ///	@brief	Will parse the argument for a number of parameter definition.
    ///	@param	arg			[in] The argument to parse.
    ///	@param	numParam	[out] The number of parameter, which will be expected.
    ///	@return	Will return true, if the syntax is valid, false if not.
    static bool parseArgParameter( const String &arg, ui32 &numPara );

    ///	@brief	Returns a blanked argument.
    ///	@param	arg	[in] The argument definition in the expected form arg<numParam>;
    ///	@return	The blanked argument.
    static String getBlankArgument( const String &arg );

    // no copying
    ArgumentParser() = delete;
    ArgumentParser(const ArgumentParser &) = delete;
    ArgumentParser &operator = (const ArgumentParser &) = delete;

protected:
    /// @brief	Will validate the args.
    /// @param	iArgc	[in] Number of incoming arguments.
    ///	@param	ppArgv	[in] The arguments.
    bool validateArguments( i32 iArgc, const c8 *ppArgv[] );
    void setInvalid();

private:
    cppcore::TArray<Argument> mSupportedArguments;	// List with supported arguments
    StringArray mDetectedArgs; // List with detected arguments
    StringArray mStoredArguments; // List with store arguments
    ui32 mCurrentIndex;					            // The current index for iteration
    String mLastError;                              // The last errors
    bool mIsValid;							        // The valid flag
};

inline void ArgumentParser::addError( const String &error ) {
    mLastError += error;
}

inline const String &ArgumentParser::getLastErrors() const {
    return mLastError;
}

inline bool ArgumentParser::hasSupportedArguments() const {
    return ( !mSupportedArguments.isEmpty() );
}

inline void ArgumentParser::reset() {
    mCurrentIndex = 0;
}

} // Namespace Common
} // Namespace OSRE
