/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

using namespace ::OSRE;

// Define compiler-specific export macro
#ifdef OSRE_WINDOWS
#  define TAG_DLL_EXPORT __declspec(dllexport)
#  define TAG_DLL_IMPORT __declspec(dllimport )
#endif 

#define OSRE_EDITOR_BUILD_EXPORT

#ifdef OSRE_WINDOWS
#   ifdef OSRE_EDITOR_BUILD_EXPORT
#       define OSRE_EDITOR_EXPORT TAG_DLL_EXPORT
#   else
#       define OSRE_EDITOR_EXPORT TAG_DLL_IMPORT
#   endif
#else
#   define OSRE_EDITOR_EXPORT  __attribute__ ((visibility("default")))
#endif

// Define the calling convention macro for each supported platform
#ifdef OSRE_WINDOWS
#   define STDCALL __stdcall
#else
#   define STDCALL
#endif

struct NativeStreeItem {
    String m_name;
    i32   m_parentId;
    i32   m_numChildren;
    i32  *m_childrenIds;
};

struct CSharpEvent {
    int type;
    int x;
    int y;
    int mouseBtnState;
};

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle );

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorUpdate();

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorRequestNextFrame();

extern "C" OSRE_EDITOR_EXPORT int STDCALL DestroyEditorApp();

extern "C" OSRE_EDITOR_EXPORT int STDCALL NewProject(const char *name);

extern "C" OSRE_EDITOR_EXPORT int STDCALL LoadWorld(const char *filelocation, int flags);

extern "C" OSRE_EDITOR_EXPORT int STDCALL SaveWorld(const char *filelocation, int flags );

extern "C" OSRE_EDITOR_EXPORT int STDCALL ImportAsset(const char *filename, int flags);

extern "C" OSRE_EDITOR_EXPORT int STDCALL GetNumItems();

extern "C" OSRE_EDITOR_EXPORT int STDCALL  GetNodeHierarchy( int numItems, NativeStreeItem *items );
