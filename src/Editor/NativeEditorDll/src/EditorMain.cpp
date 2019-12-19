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
#include "EditorMain.h"
#include "UiLogStream.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Event.h>
#include <osre/Common/Logger.h>
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

fnc_log_callback g_fnc_log_callback = nullptr;

namespace OSRE {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::NativeEditor;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Assets;

static const c8 *Tag = "EditorMain";

static OSRE::EditorApplication *s_EditorApplication = nullptr;

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle ) {
#ifdef OSRE_WINDOWS
    if (nullptr == s_EditorApplication) {
        HWND mainWH( nullptr );
        if (nullptr == mainWindowHandle) {
            osre_error(Tag, "Invalid main window handle.");
            return 1;
        }

        mainWH = (HWND) mainWindowHandle;

        char *argc[] = { 
            "CreateEditorApp" 
        };
        s_EditorApplication = new EditorApplication( 1, argc );
        s_EditorApplication->create();

        Logger *logger = Logger::getInstance();
        if (nullptr != logger) {
            logger->registerLogStream(new UiLogStream);
        }

        s_EditorApplication->update();
        Win32Window *window = (Win32Window*)s_EditorApplication->getRootWindow();
        if (nullptr != window) {
            HWND childHandle = window->getHWnd();
            if (childHandle == nullptr) {
                osre_error(Tag, "Cannot get window handle from editor instance.");
                return -1;
            }

            HWND wnd = ::SetParent( childHandle, mainWH );
            if (!wnd) {
                osre_error( Tag, "Cannot set parent window." );
                return -1;
            }

            RECT rect;
            if (TRUE != GetClientRect(mainWH, &rect)) {
                osre_error(Tag, "Cannot get geometry from parent window.");
                return -1;
            }

            const ui32 w = rect.right - rect.left;
            const ui32 h = rect.bottom - rect.top;
            if (FALSE == ::MoveWindow( childHandle, rect.left, rect.top, w, h, TRUE )) {
                osre_error( Tag, "Cannot move window." );
                return -1;
            }
        }

        ::CPPCore::TArray<const Common::Event*> eventArray;
        eventArray.add( &KeyboardButtonDownEvent );
        OSEventListener *listener = new MouseEventListener( s_EditorApplication );
        AbstractPlatformEventQueue *evQueue = s_EditorApplication->getPlatformInterface()->getPlatformEventHandler();
        if (nullptr == evQueue) {
            osre_error(Tag, "Cannot get event queue.");
            return -1;
        }

        evQueue->registerEventListener( eventArray, listener );

        s_EditorApplication->requestNextFrame();
    }
#endif // OSRE_WINDOWS
    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorUpdate() {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    s_EditorApplication->update();

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorRequestNextFrame() {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    s_EditorApplication->handleEvents();
    s_EditorApplication->requestNextFrame();

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL DestroyEditorApp() {
    if (nullptr == s_EditorApplication) {
        return 1;
    }
    
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL EditorResize(i32 x, i32 y, i32 w, i32 h) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    WindowsResizeEventData* data = new WindowsResizeEventData(nullptr );
    data->m_x = x;
    data->m_y = y;
    data->m_w = w;
    data->m_h = h;
    s_EditorApplication->resize(x, y, w, h);
    s_EditorApplication->enqueueEvent(&WindowsResizeEvent, data);
    s_EditorApplication->getRenderBackendService()->resize(x, y, w, h);
    s_EditorApplication->handleEvents();
    s_EditorApplication->requestNextFrame();

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL NewProject( const char *name ) {
    if (nullptr == name) {
        return 1;
    }

    s_EditorApplication->newProject( name );

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL LoadProject( const char *filelocation, int flags ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    const bool retValue = s_EditorApplication->loadProject( filelocation, flags );

    return retValue ? 0 : 1;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL SaveProject( const char *filelocation, int flags ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    const bool retValue = s_EditorApplication->saveProject( filelocation, flags );

    return retValue ? 0 : 1;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL ImportAsset( const char *filename, int flags ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    if (nullptr == filename) {
        return -1;
    }

    return s_EditorApplication->importAsset( filename, flags );
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL openWorldAccess( const char *name ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    if (nullptr == name) {
        return -1;
    }

    int retCode = s_EditorApplication->openWorldAccess( name );
    if (-1 != retCode) {

    }
    return retCode;
}

// Scene access
extern "C" OSRE_EDITOR_EXPORT int STDCALL openStageAccess( const char *name ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    return s_EditorApplication->openStageAccess( name );
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL openNodeAccess( const char *name ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    return s_EditorApplication->openNodeAccess( name );
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL createNode( const char *name ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }


}
extern "C" OSRE_EDITOR_EXPORT int STDCALL closeNodeAccess( void ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }
    
    return s_EditorApplication->closeNodeAccess();
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL closeStageAccess( void ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }
    return s_EditorApplication->closeStageAccess();
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL closeWorldAccess( void ) {
    if (nullptr == s_EditorApplication) {
        return -1;
    }

    return s_EditorApplication->closeWorldAccess();
}

extern "C" OSRE_EDITOR_EXPORT void STDCALL RegisterLogCallback(fnc_log_callback *fnc) {
    fnc_log_callback ptr = (fnc_log_callback)fnc;
    g_fnc_log_callback = ptr;
}

static void enqueueMouseBtnData(int x, int y, bool pressed, MouseButtonEventData::ButtonType type) {
    MouseButtonEventData* data = new MouseButtonEventData(pressed, nullptr);
    data->m_AbsX = x;
    data->m_AbsY = y;
    data->m_Button = type;
    if (pressed) {
        s_EditorApplication->enqueueEvent(&MouseButtonDownEvent, data);
    } else {
        s_EditorApplication->enqueueEvent(&MouseButtonUpEvent, data);
    }
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL LeftMousePressed(int x, int y, bool pressed) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }
    
    enqueueMouseBtnData(x, y, pressed, MouseButtonEventData::LeftButton);

    return 0;
}

extern "C" OSRE_EDITOR_EXPORT int STDCALL MiddleMousePressed(int x, int y, bool pressed) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }
    
    enqueueMouseBtnData(x, y, pressed, MouseButtonEventData::MiddleButton);

    return 0;

}

extern "C" OSRE_EDITOR_EXPORT int STDCALL RightMousePressed(int x, int y, bool pressed) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    enqueueMouseBtnData(x, y, pressed, MouseButtonEventData::RightButton);

    return 0;
}

} // Namespace OSRE
