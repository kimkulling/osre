/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Modules/IModuleView.h"
#include "OsreEdApp.h"

#include "Common/Logger.h"
#include "Common/osre_common.h"
#include "Platform/AbstractWindow.h"
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <iostream>

namespace OSRE {
namespace Editor {

static constexpr c8 Tag[] = "LogModule";

using namespace ::OSRE::Common;
using namespace ::OSRE::App;
using namespace ::OSRE::Modules;
using namespace ::OSRE::Platform;
                                
static constexpr int MarginWidth  = 20;
static constexpr int MarginHeight = 300;

/**
 * @class LogView
 * @brief Provides a module-specific implementation for managing and displaying log entries.
 *
 * The `LogView` class extends from `IModuleView` and serves as a mechanism to
 * manage log messages within a module. It provides functionalities to add, clear,
 * and display log messages. The class ensures that changes in the log data are
 * updated in real-time via the `onUpdate()` method.
 *
 * A `LogView` requires an `AbstractWindow` instance upon construction. The
 * `addEntry` method allows appending log messages, while the `clear` method
 * resets the log storage. Changes to the log are visualized through the `onUpdate`
 * call, which outputs the log data and resets after displaying.
 *
 * This class also overrides `onCreate` and `onUpdate`, ensuring integration
 * with the lifecycle of the module view.
 *
 * @note The log data is cleared after each update in `onUpdate`.
 */
class LogView final : public IModuleView {
public:
    explicit LogView(AbstractWindow *window) :
            IModuleView("logview" ),
            mText() {
        // empty
    }

    ~LogView() override = default;

    void clear() {
        mText.clear();
        onUpdate();
    }

    void addEntry(const String &text) {
        mText.append(text);
        onUpdate();
    }

protected:
    void onCreate(const Rect2ui &rect) override {
    }

    void onUpdate() override {
        std::cout << mText << std::endl;
        mText.clear();
    }

private:
    String mText;
};

class LogStream final : public AbstractLogStream {
public:
    explicit LogStream(LogView *logView) : AbstractLogStream(), mLogView(logView) {
        osre_assert(logView != nullptr);
    }

    ~LogStream() override = default;

    void write(const String &rMessage) override {
        if (mLogView != nullptr) {
            mLogView->addEntry(rMessage);
        }
    }

private:
    LogView *mLogView;
};

/**
 * @class AssimpLogStream
 * @brief Implements a custom log stream for Assimp logging to interface with a given LogView.
 *
 * This class extends the Assimp::LogStream interface, providing a mechanism
 * to redirect log messages from the Assimp library to a given `LogView` instance.
 * The log message is formatted and augmented with `(Assimp)` before being passed
 * to the `LogView` for visualization or storage.
 *
 * @note The constructor requires a valid `LogView` instance; passing a nullptr will
 * trigger an assertion failure.
 */
class AssimpLogStream final : public Assimp::LogStream {
public:
    explicit AssimpLogStream(LogView *logView) :
            LogStream(), mLogView(logView) {
        osre_assert(logView != nullptr);
    }

    ~AssimpLogStream() override = default;

    void write(const char *message) override {
        const String logMsg = String(message)+ " (Assimp)";
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
    const AppBase *parentApp = getParentApp();
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
