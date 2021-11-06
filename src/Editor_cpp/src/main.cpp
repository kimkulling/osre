#include "OsreEdApp.h"
#include <osre/Platform/AbstractOSService.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Common/Environment.h>
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Editor;
using namespace ::OSRE::Platform;

#include <osre/Platform/Windows/MinWindows.h>

void getMonitorResolution(ui32 &width, ui32 &heigt) {
#ifdef OSRE_WINDOWS
    width = GetSystemMetrics(SM_CXSCREEN);
    heigt = GetSystemMetrics(SM_CYSCREEN);
#else
    width = heigt = 0;
#endif
}

int main(int argc, char *argv[]) {
    OsreEdApp app(argc, argv);

    const ui32 Margin = 100;
    ui32 width = 0, height = 0;
    getMonitorResolution(width, height);
    width -= Margin;
    height -= Margin;
    if (!app.initWindow(10, 10, width, height, "OSRE-Ed", false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    Common::Environment *env = app.getEnvironment();
    if (nullptr == env) {
        return 1;
    }

    env->addIntVar("MainWindow.width", width);
    env->addIntVar("MainWindow.height", height);


    while (app.handleEvents()) {
        app.update();
        app.requestNextFrame();
    }

    MemoryStatistics::showStatistics();

    return 0;
}
