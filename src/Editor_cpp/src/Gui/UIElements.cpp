/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "UIElements.h"
#include "OsreEdApp.h"
#include <osre/RenderBackend/MeshBuilder.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/Platform/PlatformInterface.h>

#include <commoncontrols.h>
#include <winuser.h>

#ifdef OSRE_WINDOWS
#include "Engine/Platform/win32/Win32EventQueue.h"
#include "Engine/Platform/win32/Win32Window.h"
#include "shellapi.h"
#include <CommCtrl.h>
#include <commctrl.h>
#include <strsafe.h>
#include <windows.h>
#include <winuser.h>
#endif

namespace OSRE {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Editor;

constexpr int IDM_FILE_NEW = 1;
constexpr int IDM_FILE_OPEN = 2;
constexpr int IDM_FILE_SAVE = 3;
constexpr int IDM_FILE_IMPORT = 4;
constexpr int IDM_FILE_QUIT = 5;

constexpr int IDM_MESH_NEW = 100;

constexpr int IDM_GETTING_HELP = 6;
constexpr int IDM_INFO_VERSION = 7;

String TreeItem::generateName() {
    return "newItem_" + std::to_string(mChildren.size());
}

TreeItem::TreeItem() : mName(generateName()), mParent(nullptr), mChildren() {}

void TreeItem::addChild(TreeItem *item) {
    if (item == nullptr) {
        return;
    }
    
    mChildren.add(item);
}

size_t TreeItem::numChildren() const {
    return mChildren.size();
}
    
TreeItem *TreeItem::getParent() const {
    return mParent;
}

ProgressBar *UIElements::createProgressBar(int id, HWND hWnd, const Rect2ui &rect) {
    ProgressBar *pb = new ProgressBar;
    pb->mPlatformData.mHWnd = CreateWindowEx(
            0,
            PROGRESS_CLASS,
            (LPSTR)NULL,
            WS_VISIBLE | WS_CHILD,
            rect.getX1(),
            rect.getY1(),
            rect.getWidth(),
            rect.getHeight(),
            hWnd,
            (HMENU)id,
            (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
            NULL);

    if (!pb->mPlatformData.mHWnd) {
        MessageBox(NULL, "Progress Bar Failed.", "Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }
    pb->mRange=100;
    pb->mCurrent = 0;
    
    auto wnd = pb->mPlatformData.mHWnd;
    ::SendMessage(hWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 99));
    ::SendMessage(hWnd, PBM_SETSTEP, (WPARAM)1, 0);

    pb->mParent = nullptr;
    pb->mRect = rect;

    return pb;
}

void UIElements::updateProgressBar(ProgressBar *pb, ui32 step) {
    if (pb == nullptr) {
        return;
    }
    ::SendMessage(pb->mPlatformData.mHWnd, PBM_SETPOS, (WPARAM)step, 0);
}
    
void UIElements::deleteProgressBar(ProgressBar *pb) {
    if (pb == nullptr) {
        return;
    }
    ::CloseWindow(pb->mPlatformData.mHWnd);
    delete pb;
}

void UIElements::createMenues(Win32Window *w, OsreEdApp *app, Platform::AbstractPlatformEventQueue *queue) {
    osre_assert(w != nullptr);
    osre_assert(app != nullptr);
    osre_assert(queue != nullptr);

    w->beginMenu();
    MenuEntry FileMenu[8] = {
        { MF_STRING, IDM_FILE_NEW, L"&New", MenuFunctor::Make(app, &OsreEdApp::newProjectCmd) },
        { MF_STRING, IDM_FILE_OPEN, L"&Open Project", MenuFunctor::Make(app, &OsreEdApp::loadProjectCmd) },
        { MF_STRING, IDM_FILE_SAVE, L"&Save Project", MenuFunctor::Make(app, &OsreEdApp::saveProjectCmd) },
        { MF_SEPARATOR, 0, nullptr },
        { MF_STRING, IDM_FILE_IMPORT, L"&Import Asset", MenuFunctor::Make(app, &OsreEdApp::importAssetCmd) },
        { MF_SEPARATOR, 0, nullptr },
        { MF_STRING, IDM_FILE_QUIT, L"&Quit", MenuFunctor::Make(app, &OsreEdApp::quitEditorCmd) },
    };
    w->addSubMenues(nullptr, queue, L"File", FileMenu, 8);

    MenuEntry SceneMenu[1] = {
        { MF_STRING, IDM_MESH_NEW, L"&New Mesh", MenuFunctor::Make(app, &OsreEdApp::newMeshCmd) },
    };
    w->addSubMenues(nullptr, queue, L"Scene", SceneMenu, 1);

    MenuEntry InfoMenu[2] = {
        { MF_STRING, IDM_GETTING_HELP, L"&Getting Help", MenuFunctor::Make(app, &OsreEdApp::gettingHelpCmd) },
        { MF_STRING, IDM_INFO_VERSION, L"&Version", MenuFunctor::Make(app, &OsreEdApp::showVersionCmd) }
    };
    w->addSubMenues(nullptr, queue, L"&Info", InfoMenu, 2);

    w->endMenu();
}

TreeView *UIElements::createTreeView(Win32Window *w) {
    if (w == nullptr) {
        return nullptr;
    }
}

} // namespace OSRE
