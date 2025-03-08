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

namespace OSRE::Debugging {
    /**
     * @brief Handles fatal assertion failures and outputs detailed information for debugging purposes.
     *
     * This function is called when a fatal assertion failure occurs. It prints the provided
     * debugging information including the file name, line number, and a custom error message.
     *
     * @param file The name of the file where the assertion failure occurred.
     * @param line The line number in the file where the assertion failure occurred.
     * @param msg A string containing the custom error message or description of the failure.
     */
    OSRE_EXPORT void handleFatal(const String &file, int line, const String &msg);

    /**
     * @brief Handles assertion failures by invoking the fatal error handler, breaking into the debugger, and terminating the program.
     *
     * This function is called when an assertion failure occurs. It passes the provided file name, line number, and error message
     * to the fatal error handler for detailed debugging information. Additionally, it triggers a break for debugging purposes and
     * terminates the program execution.
     *
     * @param file The name of the file where the assertion failure occurred.
     * @param line The line number in the file where the assertion failure occurred.
     * @param msg A string containing the error message or description of the assertion failure.
     */
    OSRE_EXPORT void handleAssert( const String &file, int line, const char *msg );

} // Namespace OSRE::Debugging
