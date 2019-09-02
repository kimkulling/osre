/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLCommon.h"

#include <osre/Common/Logger.h>

namespace OSRE {
namespace RenderBackend {

static const c8 *Tag = "OGLRenderBackend";

ui32 OGLRenderCmdAllocator::m_lastid( 0 );

void checkOGLErrorState( const c8 *file, ui32 line ) {
	GLenum error = glGetError();
	if (GL_NO_ERROR == error) {
		return;
	}

	switch (error) {
	    case GL_INVALID_ENUM:
		    // An unacceptable value is specified for an enumerated argument. The offending command 
		    // is ignored and has no other side effect than to set the error flag.
		    ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_ENUM error." );
		    break;

	    case GL_INVALID_VALUE:
		    // A numeric argument is out of range. The offending command is ignored and has no other 
		    // side effect than to set the error flag.
		    ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_VALUE error." );
		    break;

	    case GL_INVALID_OPERATION:
		    //The specified operation is not allowed in the current state. The offending command is 
		    //ignored and has no other side effect than to set the error flag.
		    ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_OPERATION error." );
		    break;

	    case GL_INVALID_FRAMEBUFFER_OPERATION:
		    // The command is trying to render to or read from the framebuffer while the currently bound 
		    // framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is
		    // not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than
		    // to set the error flag.
		    ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_FRAMEBUFFER_OPERATION error." );
		    break;

	    case GL_OUT_OF_MEMORY:
		    // There is not enough memory left to execute the command. The state of the GL is undefined, except 
		    // for the state of the error flags, after this error is recorded.
		    ::OSRE::Common::errorPrint( Tag, file, line, "GL_OUT_OF_MEMORY error." );
		    break;

	    default:
		    break;
	}
}

} // Namespace RendeBackend
} // Namespace OSRE
