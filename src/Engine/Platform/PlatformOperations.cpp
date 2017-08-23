/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#ifdef OSRE_WINDOWS
#   include <Windows.h>
#   include <Commdlg.h>
#endif // OSRE_WINDOWS

namespace OSRE {
namespace Platform {
PlatformOperations::PlatformOperations() {
    // empty
}

PlatformOperations::~PlatformOperations() {
    // empty
}

void PlatformOperations::getFileOpenDialog( const String &extensions, IO::Uri &location ) {
#ifdef OSRE_WINDOWS
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[ 260 ];       // buffer for file name
    //HWND hwnd;              // owner window

                            // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    //ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = extensions.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if ( TRUE == GetOpenFileName( &ofn ) ) {
        String filename = ofn.lpstrFile;
        location.setResource( filename );
    } else {
        location.clear();
    }
#endif // OSRE_WINDOWS
}

void PlatformOperations::getFileSaveDialog( const String &extensions, IO::Uri &location ) {
#ifdef OSRE_WINDOWS
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[ 260 ];       // buffer for file name
                              //HWND hwnd;              // owner window

                              // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    //ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = extensions.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST;

    // Display the Open dialog box. 
    if ( TRUE == GetSaveFileName( &ofn ) ) {
        String filename = ofn.lpstrFile;
        location.setResource( filename );
    } else {
        location.clear();
    }
#endif // OSRE_WINDOWSl

}

} // Namespace Platform
} // Namespace OSRE
