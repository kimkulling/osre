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

#include "Platform/PlatformCommon.h"

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace IO {
    class Uri;
}

namespace Platform {


///	@brief  Enumerator the a dialog result.
enum class DlgResults {
    DlgButtonRes_Yes = 1,   ///< Yes-button
    DlgButtonRes_No,        ///< No-button
    DlgButtonRes_Ok         ///< Ok-button
};

class OSRE_EXPORT PlatformOperations {
public:
    enum {
        DlgButton_YesNo = 1,
        DlgButton_ok = 2
    };

    static void getFileOpenDialog( const String &title, const c8 *extensions, IO::Uri &location );
    static void getFileSaveDialog(const String &title, const c8 *extensions, IO::Uri &location);
    static void getDialog( const String &title, const String &question, ui32 requestedButtons, DlgResults &result );
    static i32 executeCommand();
    
    PlatformOperations() = delete;
    PlatformOperations(const PlatformOperations &) = delete;
    ~PlatformOperations() = delete;
};

} // Namespace Platform
} // Namespace OSRE
