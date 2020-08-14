#include "OsreEdApp.h"
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::UI;
using namespace ::OSRE::Editor;

int main(int argc, char *argv[]) {
    OsreEdApp app(argc, argv);
    if (!app.initWindow(10, 10, 1024, 768, "ModelLoader-Sample", false, App::RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (app.handleEvents()) {
        app.update();
        if (app.hasModel()) {
            app.requestNextFrame();
        }
    }

    return 0;
}
