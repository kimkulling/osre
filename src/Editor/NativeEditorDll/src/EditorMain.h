#pragma once

#include <osre/Common/osre_common.h>

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

extern "C" OSRE_EDITOR_EXPORT int CreateEditorApp( int *mainWindowHandle );

extern "C" OSRE_EDITOR_EXPORT int EditorUpdate();

extern "C" OSRE_EDITOR_EXPORT int EditorRequestNextFrame();

extern "C" OSRE_EDITOR_EXPORT int DestroyEditorApp();
