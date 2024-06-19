/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <cassert>

#include "Common/Logger.h"
#include "Common/osre_common.h"
#include "Debugging/AssertHandler.h"

//-------------------------------------------------------------------------------------------------
///	@def	OSRE_ASSERT
///	@brief	The Open-Source-rendering-Engine assert macro. The system will exit in case of an assertion. 
///	        _Debug must be defined to have support. In release macro will be ignored.
///	@param	statement	[in] The statement to validate. Must return true.
///
///	@def	OSRE_VALIDATE
///	@brief	The Open-Source-rendering-Engine validation macro. _Debug must be defined to have support.
///	        _Debug must be defined to have support. In release macro will be ignored.
///	@param	statement	[in] The statement to validate. Must return true.
///	@param	msg	        [in] The error message to show in case of an error.
///
/// @def    OSRE_CHECK_NOENTRY
/// @brief  If you want to validate a statement no to be performed use this macro. It will abort 
///         the application.
//-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#  define osre_assert( statement )        if ( !(statement) ) ::OSRE::Debugging::handleAssert( __FILE__, __LINE__,  #statement );
#  define osre_assert2( statement, msg )  if ( !(statement) ) ::OSRE::Debugging::handleAssert( __FILE__, __LINE__,  #statement###msg );
#  define osre_validate( statement, msg ) if ( !(statement) ) ::OSRE::Debugging::handleFatal( __FILE__, __LINE__,  ( msg ) );
#  define OSRE_CHECK_NOENTRY()                                ::OSRE::Debugging::handleAssert( __FILE__, __LINE__, "Happened!" );
#  define OSRE_CHECK_NOENTRY2( msg )                          ::OSRE::Debugging::handleAssert( __FILE__, __LINE__, ( msg ) );
#else
#  define osre_assert( statement )
#  define osre_assert2( statement, msg )  if ( !(statement) ) (::OSRE::Common::warnPrint( __FILE__ ,__FILE__, __LINE__, msg ) );
#  define osre_validate( statement, msg ) statement
#  define OSRE_CHECK_NOENTRY()
#  define OSRE_CHECK_NOENTRY2( msg )
#endif
