#pragma once

#include <osre/Common/osre_common.h>

using namespace ::OSRE;

#ifdef OSRE_WINDOWS
#  define TAG_DLL_EXPORT __declspec(dllexport)
#  define TAG_DLL_IMPORT __declspec(dllimport )
#endif 

// Define compiler-specific export macro
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
    ui32 m_parentId;
    String m_name;
    ui32 m_numChildren;
    ui32 *m_childrenIds;
};

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle );

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorUpdate();

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorRequestNextFrame();

extern "C" OSRE_EDITOR_EXPORT int STDCALL DestroyEditorApp();

extern "C" OSRE_EDITOR_EXPORT int STDCALL LoadWorld(const char *filelocation, int flags);

extern "C" OSRE_EDITOR_EXPORT int STDCALL SaveWorld(const char *filelocation, int flags );

extern "C" OSRE_EDITOR_EXPORT int STDCALL ImportAsset(const char *filename, int flags);
