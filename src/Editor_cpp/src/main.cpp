#include <osre/App/AppBase.h>
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::UI;

class OsreEdApp : public App::AppBase {
public:
    OsreEdApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api", "The render API"),
            m_canvas(nullptr){
        // empty
    }

    ~OsreEdApp() override {
        // empty
    }

protected:
    bool onCreate() override {

        return true;
    }

    void onUpdate() override {

    }

    bool onDestroy() override {
        return true;
    }

private:
    Canvas *m_canvas;
};

int main(int argc, char *argv[]) {
    std::cout << "Hello, editor.\n";
    return 0;
}
