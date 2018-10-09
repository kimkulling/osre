#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Event.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include "EditorApplication.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#   include <src/Engine/Platform/win32/Win32Window.h>
#endif // OSRE_WINDOWS

#include "SceneNodeIterator.h"

namespace OSRE {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::NativeEditor;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Assets;

static const String Tag = "EditorMain";

static OSRE::EditorApplication *s_EditorApplication = nullptr;

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle ) {
#ifdef OSRE_WINDOWS
    if (nullptr == s_EditorApplication) {
        HWND mainWH( nullptr );
        if (nullptr != mainWindowHandle) {
            mainWH = (HWND)mainWindowHandle;
        }
        char *argc[] = { "CreateEditorApp" };
        s_EditorApplication = new EditorApplication( 1, argc );
        s_EditorApplication->create();
        s_EditorApplication->update();

        Win32Window *window = (Win32Window*)s_EditorApplication->getRootWindow();
        if (nullptr != window) {
            HWND childHandle = window->getHWnd();
            ::SetParent( childHandle, mainWH );
            RECT rect;
            ::GetClientRect( mainWH, &rect );
            const ui32 w = rect.right - rect.left;

            ::MoveWindow( childHandle, 25, 45, w - 240, rect.bottom - 45, TRUE );
        }

        ::CPPCore::TArray<const Common::Event*> eventArray;
        eventArray.add( &KeyboardButtonDownEvent );
        OSEventListener *listener = new MouseEventListener( s_EditorApplication );
        AbstractPlatformEventQueue *evQueue = s_EditorApplication->getPlatformInterface()->getPlatformEventHandler();
        evQueue->registerEventListener( eventArray, listener );

        s_EditorApplication->requestNextFrame();

    }
#endif // OSRE_WINDOWS
    return 0;
}

int STDCALL EditorUpdate() {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    s_EditorApplication->update();

    return 0;
}

int STDCALL EditorRequestNextFrame() {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    s_EditorApplication->requestNextFrame();

    return 0;
}

int STDCALL DestroyEditorApp() {
    if (nullptr == s_EditorApplication) {
        return 1;
    }
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}

int STDCALL NewProject( const char *name ) {
    if (nullptr == name) {
        return 1;
    }
    s_EditorApplication->newProject( name );

    return 0;
}

int STDCALL LoadWorld( const char *filelocation, int flags ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    const bool retValue = s_EditorApplication->loadWorld( filelocation, flags );

    return retValue ? 0 : 1;
}

int STDCALL SaveWorld( const char *filelocation, int flags ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    const bool retValue = s_EditorApplication->saveWorld( filelocation, flags );

    return retValue ? 0 : 1;
}

int STDCALL ImportAsset( const char *filename, int flags ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filename) {
        return 1;
    }

    return s_EditorApplication->importAsset( filename, flags );
}

}
