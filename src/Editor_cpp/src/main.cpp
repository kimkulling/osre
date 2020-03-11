#include <osre/App/AppBase.h>

#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;

class OsreEdApp : public App::AppBase {

public:
    OsreEdApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api:model", "The render API:The model to load") {
        // empty
    }

    ~OsreEdApp() override {
        // empty
    }

};

int main(int argc, char *argv[]) {
    std::cout << "Hello, editor.\n";
    return 0;
}
