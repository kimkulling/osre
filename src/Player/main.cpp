#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractDynamicLoader.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::App;

static const c8 *Tag = "player";

class PlayerApplication : public AppBase {
public:
    PlayerApplication(int argc, c8 *argv[]) :
            AppBase(argc, (const char **)argv) {
        // empty
    }

    ~PlayerApplication() override {
        // empty
    }

protected:
    bool onCreate() override {
        const ArgumentParser &ap = AppBase::getArgumentParser();
        String dllName = ap.getArgument("run_dll");
        if (dllName.empty()) {
            osre_error(Tag, "Dll name is empty.");
            osre_info(Tag, ap.showHelp());
            return false;
        }

        if (!AppBase::onCreate()) {
            return false;
        }

        Platform::AbstractDynamicLoader *dynLoader = Platform::PlatformInterface::getInstance()->getDynamicLoader();
        Platform::LibHandle *lib = dynLoader->load(dllName);
        if (lib == nullptr) {
            osre_error(Tag, "Cannot load library " + dllName + ".");
            return false;
        }

        return true;
    }

    bool onDestroy() override {
        return AppBase::onDestroy();
    }

    void onRender() override {

    }

    void onUpdate() override {

    }
};

int main(int argc, char *argv[]) {
    PlayerApplication playerApplication(argc, argv);
    if (!playerApplication.create()) {
        osre_error(Tag, "Error while creating player application");
        return 1;
    }

    while (playerApplication.handleEvents()) {
        playerApplication.update();
        playerApplication.requestNextFrame();
    }
    playerApplication.destroy();

    return 0;
}
