#include "OsreEdApp.h"
#include <osre/Platform/AbstractOSService.h>
#include <osre/Platform/PlatformInterface.h>
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

    const ui32 Margin = 10;
    ui32 width = 0, height = 0;
    getMonitorResolution(width, height);
    if (!app.initWindow(Margin, Margin, width - Margin, height - Margin, "OSRE-Ed", false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (app.handleEvents()) {
        app.update();
        app.requestNextFrame();
    }

    MemoryStatistics::showStatistics();

    return 0;
}
