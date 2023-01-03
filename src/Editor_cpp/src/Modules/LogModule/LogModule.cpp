/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Modules/LogModule/LogModule.h"
#include <osre/Modules/IModuleView.h>
#include "OsreEdApp.h"

#include <osre/Common/Logger.h>
#include <osre/Common/osre_common.h>
#include <osre/Common/Logger.h>
#include <osre/Platform/AbstractWindow.h>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "richedit.h"
#include "src/Engine/Platform/win32/Win32Window.h"
#include "src/Engine/Platform/win32/Win32OSService.h"
#include "resource.h"

namespace OSRE {
namespace Editor {

static constexpr c8 Tag[] = "LogModule";

using namespace ::OSRE::Common;
using namespace ::OSRE::App;
using namespace ::OSRE::Modules;
using namespace ::OSRE::Platform;
                                
INT_PTR CALLBACK LogDialogProc(HWND hWnd, UINT uMsg, WPARAM /*wParam*/, LPARAM lParam) {
    (void)lParam;
    switch (uMsg) {
        case WM_INITDIALOG:
            return TRUE;

        case WM_SIZE: {
            const int x = LOWORD(lParam);
            const int y = HIWORD(lParam);

            ::SetWindowPos(GetDlgItem(hWnd, IDC_EDIT1), nullptr, 10, 10,
                    x - 10, y - 12, SWP_NOMOVE | SWP_NOZORDER);

            return TRUE;
        }
        case WM_CLOSE:
            ::CloseWindow(hWnd);
            return TRUE;
    };

    return FALSE;
}

static constexpr int MarginWidth  = 20;
static constexpr int MarginHeight = 300;

static HWND initLogWindow(HINSTANCE hInstance, HWND hParent, const Rect2ui &rect) {
    HWND hwnd = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LOGVIEW),
            hParent, &LogDialogProc);
    if (hwnd == nullptr) {
        osre_error(Tag, "Cannot create log window.");
        return nullptr;
    }

    ::MoveWindow(hwnd, rect.getX1() + MarginWidth, rect.getY2() - 300, rect.getX2() - rect.getX1() - MarginWidth, 280, TRUE);
    if (hwnd == nullptr) {
        auto err = Win32OSService::getLastErrorAsString();
        osre_error(Tag, "Unable to create log window. Error : " + err);
        return nullptr;
    }

    return hwnd;
}

class LogView : public IModuleView {
public:
    LogView(Platform::AbstractWindow *window) :
            IModuleView("logview" ),
            mText(),    
            mRect(),
            mRootWindow(window),
            mLogWndHandle(nullptr) {
        // empty
    }

    ~LogView() override = default;

    void clear() {
        mText.clear();
        onUpdate();
    }

    void addEntry(String text) {
        mText.append(text);
        mText.append("\r\n");
        onUpdate();
    }

protected:
    void onCreate(const Rect2ui &rect) override {
        Win32Window *w = (Win32Window *)mRootWindow;
        if (w == nullptr) {
            osre_error(Tag, "Cannot create log module view.");
            return;
        }

        mLogWndHandle = initLogWindow(w->getModuleHandle(), w->getHWnd(), rect);
        if (mLogWndHandle == nullptr) {
            osre_error(Tag, "Cannot create log module view.");
            return;
        }
        
        ::ShowWindow(mLogWndHandle, SW_SHOW);
        mRect = rect;
    }

    void onUpdate() override {
        SETTEXTEX sInfo = {};
        sInfo.flags = ST_DEFAULT;
        sInfo.codepage = CP_ACP;
        ::SendDlgItemMessage(mLogWndHandle, IDC_EDIT1, EM_SETTEXTEX, (WPARAM)&sInfo, (LPARAM)mText.c_str());
    }

private:
    String mText;
    Platform::AbstractWindow *mRootWindow;
    Rect2ui mRect;
    HWND mLogWndHandle;
};

class LogStream : public AbstractLogStream {
public:
    LogStream(LogView *lv) :
            AbstractLogStream(), mThreadId(999999), mLogView(lv) {
        osre_assert(mLogView != nullptr);

        mThreadId = ::GetCurrentThreadId();
    }

    ~LogStream() override = default;

    void write(const String &rMessage) override {
        if (mThreadId == ::GetCurrentThreadId()) {
            if (mLogView != nullptr) {
                mLogView->addEntry(rMessage);
            }
        }
    }

private:
    DWORD mThreadId;
    LogView *mLogView;
};

class AssimpLogStream : public Assimp::LogStream {
public:
    AssimpLogStream(LogView *logView) :
            LogStream(), mLogView(logView) {
        osre_assert(logView != nullptr);
    }

    ~AssimpLogStream() = default;

    void write(const char *message) override {
        String logMsg = String(message)+ " (Assimp)";
        mLogView->addEntry(logMsg);
    }

private:
    LogView *mLogView;
};

LogModule::LogModule(AppBase *parentApp) :
        ModuleBase("log.module", parentApp),
        mLogView(nullptr),
        mLogStream(nullptr),
        mAssimpLogStream(nullptr) {
    // empty
}
                
LogModule::~LogModule() {
    if (nullptr != mLogView) {
        osre_error(Tag, "LogModule not unloaded before release.");
        onUnload();
    }
}

bool LogModule::onLoad() {
    AppBase *parentApp = getParentApp();
    Platform::AbstractWindow *rootWindow = parentApp->getRootWindow();
    if (nullptr == rootWindow) {
        return true;
    }

    mLogView = new LogView(rootWindow);
    Rect2ui rect;
    rootWindow->getWindowsRect(rect);
    mLogView->create(rect);
    mLogStream = new LogStream(mLogView);
    Common::Logger::getInstance()->registerLogStream(mLogStream);

    Assimp::DefaultLogger::create("log.txt");
    mAssimpLogStream = new AssimpLogStream(mLogView);
    Assimp::DefaultLogger::get()->attachStream(mAssimpLogStream);

    return true;
}

bool LogModule::onUnload() {
    Logger::getInstance()->unregisterLogStream(mLogStream);
    delete mLogStream;
    mLogStream = nullptr;
    Assimp::DefaultLogger::get()->detachStream(mAssimpLogStream);
    delete mAssimpLogStream;
    mAssimpLogStream = nullptr;
    delete mLogView;
    mLogView = nullptr;
    Assimp::DefaultLogger::kill();

    return true;
}

void LogModule::onUpdate() {
    // empty
}

} // namespace Editor
} // namespace OSRE
