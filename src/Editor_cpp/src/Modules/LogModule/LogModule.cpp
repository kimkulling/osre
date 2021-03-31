#include "Modules/LogModule/LogModule.h"
#include "Modules/IModuleView.h"
#include <osre/Common/Logger.h>
#include "OsreEdApp.h"
#include <osre/Common/osre_common.h>
#include <osre/Common/Logger.h>
#include <osre/Platform/AbstractWindow.h>

//#include <osre/Platform/Windows/MinWindows.h>
#include "windows.h"
#include "winuser.h"
#include <vector>
#include <osre/Platform/Windows/MinWindows.h>

#include "src/Engine/Platform/win32/Win32Window.h"

namespace OSRE {
namespace Editor {

static const c8 *Tag = "LogModule";

using namespace OSRE::App;
using namespace OSRE::Platform;

#define IDC_LOG 1024

INT_PTR CALLBACK LogDialogProc(HWND hWnd, UINT uMsg, WPARAM /*wParam*/, LPARAM lParam) {
    (void)lParam;
    switch (uMsg) {
        case WM_INITDIALOG:
            return TRUE;

        case WM_SIZE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            SetWindowPos(GetDlgItem(hWnd, IDC_LOG), nullptr, 0, 0,
                    x - 10, y - 12, SWP_NOMOVE | SWP_NOZORDER);

            return TRUE;
        }
        case WM_CLOSE:
            CloseWindow(hWnd);
            return TRUE;
    };

    return FALSE;
}

HWND initLogWindow(HWND hParent, HINSTANCE hInstance) {
    HWND hWnd = ::CreateWindow("Log",
            NULL,
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            10,
            10,
            100,
            20,
            hParent,
            NULL,
            hInstance,
            NULL);

    return hWnd;
}

enum class Severity {
    Info,
    Warn,
    Error
};

namespace Details {

static Color4 getColorBySeverity( Severity sev ) {
    Color4 col;
    switch (sev) {
        case Severity::Info:
            col.m_r = col.m_g = col.m_b = 1;
            break;
        case Severity::Warn:
            col.m_r = col.m_g = 1;
            break;
        case Severity::Error:
            col.m_r=1;
            break;
            
        default:
            break;
    }
    return col;
}

}

struct LogEntry {
    LogEntry() :
            mText(),
            mColor(0xFF, 0xFF, 0xFF, 0x00),
            mStartTicks(0) {
        // empty
    }

    std::string mText;
    Color4 mColor;
    ui32 mStartTicks;
};

class LogView : public IModuleView {
public:
    LogView(Platform::AbstractWindow *window) :
            IModuleView( "logview" ),
            mEntries(),
            mRect(),
            mRootWindow(window),
            mLogWndHandle(nullptr) {
        // empty
    }

    ~LogView() override {
        mEntries.clear();
    }

    void addEntry(Severity sev, String text, ui32 time) {
        LogEntry entry;
        entry.mColor = Details::getColorBySeverity(sev);
        entry.mText = text;
        entry.mStartTicks = time;
        mEntries.emplace_back(entry);
    }

protected:
    void onCreate( Rect2ui rect ) override {
        Win32Window *w = (Win32Window *)mRootWindow;
        mLogWndHandle = initLogWindow(w->getHWnd(), w->getModuleHandle());
        mRect = rect;
    }

    void onUpdate() override {
        Win32Window *w = (Win32Window *)mRootWindow;
        for (size_t i = 0; i < mEntries.size(); ++i) {
            const LogEntry &entry = mEntries[i];
            HDC dc = GetDC(mLogWndHandle);
            RECT rect;
            DrawText(dc, entry.mText.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        }
    }

private:
    std::vector<LogEntry> mEntries;
    Platform::AbstractWindow *mRootWindow;
    Rect2ui mRect;
    HWND mLogWndHandle;
};

LogModule::LogModule(AppBase *parentApp) :
        ModuleBase( "log.module", parentApp ),
        mLogView(nullptr) {
    // empty
}
                
LogModule::~LogModule() {
    if (nullptr != mLogView) {
        osre_error(Tag, "LogModule not unloaded before release.");
        delete mLogView;
    }
}

bool LogModule::onLoad() {
    AppBase *parentApp = getParentApp();
    Platform::AbstractWindow *rootWindow = parentApp->getRootWindow();
    if (nullptr == rootWindow) {
        return true;
    }

    mLogView = new LogView(rootWindow);

    return true;
}

bool LogModule::onUnload() {
    delete mLogView;
    mLogView = nullptr;

    return true;
}

void LogModule::onUpdate() {
    mLogView->update();
}

} // namespace Editor
} // namespace OSRE
