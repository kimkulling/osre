#include "OsreEdApp.h"
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::UI;
using namespace ::OSRE::Editor;

int main(int argc, char *argv[]) {
    OsreEdApp app(argc, argv);

    if (!app.create()) {
        return 1;
    }

    while (app.handleEvents()) {
        app.update();
        app.requestNextFrame();
    }

    return 0;
}
