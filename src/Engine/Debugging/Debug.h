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
#pragma once

#include "Common/osre_common.h"

namespace OSRE::Debugging {
    /**
     * @brief Triggers a programmatic debug break.
     *
     * This function is used to invoke a debug break in the running application.
     * The behavior is platform-specific:
     * - On Windows (_MSC_VER), it uses the `__debugbreak` intrinsic.
     * - On ARM CPUs (BX_CPU_ARM), it uses an assembly interrupt `int $3`.
     *
     * Typically used for debugging scenarios to halt the program and allow the debugger
     * to interrupt execution at the point of invocation.
     */
    OSRE_EXPORT void debugBreak();

    /**
     * @brief Checks if a debugger is currently attached to the running process.
     *
     * This function determines whether a debugger is attached to the application.
     * On Windows platforms, it uses the `IsDebuggerPresent` function to check for the presence of a debugger.
     * On non-Windows platforms, it always returns false as the functionality is not implemented.
     *
     * @return Returns true if a debugger is attached; otherwise, returns false.
     */
    OSRE_EXPORT bool isDebuggerAttached();

} // Namespace OSRE::Debugging

