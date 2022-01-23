#include <osre/App/AppBase.h>

using namespace ::OSRE;
using namespace ::OSRE::App;

class PlayerApplication : public AppBase {
public:
    PlayerApplication(int argc, c8 *argv[]) :
            AppBase(argc, (const char **)argv) {}

    ~PlayerApplication() override {

    }
};

int main(int argc, char *argv[]) {
    PlayerApplication playerApplication(argc, argv);
    if (!playerApplication.create()) {
        return 1;
    }
    while (playerApplication.handleEvents()) {
        playerApplication.update();
        playerApplication.requestNextFrame();
    }
    playerApplication.destroy();

    return 0;
}