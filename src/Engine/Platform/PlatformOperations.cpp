/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/PlatformOperations.h>
#include <osre/IO/Uri.h>
#include <osre/Common/Logger.h>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#   include <Commdlg.h>
#endif // OSRE_WINDOWS

namespace OSRE {
namespace Platform {

static const c8 *Tag = "PlatformOperations";

void PlatformOperations::getFileOpenDialog( const c8 *extensions, IO::Uri &location ) {
#ifdef OSRE_WINDOWS
    OPENFILENAME ofn;  
    char szFile[ 260 ];

    // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.lpstrFile = szFile;
    
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = extensions;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 
    if ( TRUE == GetOpenFileName( &ofn ) ) {
        String filename = ofn.lpstrFile;
        location.setResource( filename );
        location.parse();
    } else {
        location.clear();
    }
#else
    osre_warn( Tag, "Not supported," );
#endif // OSRE_WINDOWS
}

void PlatformOperations::getFileSaveDialog( const c8 *extensions, IO::Uri &location ) {
#ifdef OSRE_WINDOWS
    OPENFILENAME ofn;
    char szFile[ 260 ];

    // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.lpstrFile = szFile;

    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = extensions;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST;

    // Display the Open dialog box. 
    if ( TRUE == GetSaveFileName( &ofn ) ) {
        String filename = ofn.lpstrFile;
        location.setResource( filename );
    } else {
        location.clear();
    }
#else
    osre_warn(Tag, "Not supported,");
#endif // OSRE_WINDOWS
}

void PlatformOperations::getDialog( const String &title, const String &question, ui32 requestedButtons, DlgResults &result ) {
#ifdef OSRE_WINDOWS
    ui32 flags = 0;
    if ( requestedButtons & PlatformOperations::DlgButton_YesNo )  {
        flags |= MB_YESNO;
    }
    if ( requestedButtons & PlatformOperations::DlgButton_ok ) {
        flags = MB_OK;
    }

    int msgboxID = ::MessageBox(
        NULL,
        question.c_str(),
        title.c_str(),
        flags
    );

    switch ( msgboxID ) {
        case IDYES:
            result = DlgResults::DlgButtonRes_Yes;
            break;
        case IDNO:
            result = DlgResults::DlgButtonRes_No;
            break;
        case IDOK:
            result = DlgResults::DlgButtonRes_Ok;
            break;
        default:
            osre_debug( Tag, "Unsupported id detected " + std::to_string( msgboxID ) );
            break;
    }
#else
    osre_warn( Tag, "Not supported," );
#endif // OSRE_WINDOWS
}

} // Namespace Platform
} // Namespace OSRE
