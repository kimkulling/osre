/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>
#include <osre/Common/osre_common.h>

static void handleFatal( const OSRE::String &file, int line, OSRE::String &msg ) {
    OSRE::Common::fatalPrint( "Assertion", file, line, msg );
}

static void handleAssert( const OSRE::String &file, int line, OSRE::String &msg ) {
    handleFatal( file, line, msg );

#ifdef OSRE_WINDOWS
    __debugbreak();
#endif

}

#ifdef _DEBUG
#  define osre_assert( statement )        if ( !(statement) ) handleAssert( __FILE__, __LINE__,  msg );
#  define osre_validate( statement, msg ) if ( !(statement) ) handleFatal( __FILE__, __LINE__,  msg );
#else
#  define osre_assert( statement )
#  define osre_validate( statement, msg ) 
#endif

