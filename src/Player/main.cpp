#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractDynamicLoader.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::App;

static const c8 *Tag = "player";

typedef void(*createModuleFn)(void);

class PlayerApplication : public AppBase {
public:
    PlayerApplication(int argc, c8 *argv[]) :
            AppBase(argc, (const char **)argv),
            mDllName(),    
            mModule(nullptr) {
        // empty
    }

    ~PlayerApplication() override {
        // empty
    }

protected:
    bool onCreate() override {
        const ArgumentParser &ap = AppBase::getArgumentParser();
        mDllName = ap.getArgument("run_dll");
        if (mDllName.empty()) {
            osre_error(Tag, "Dll name is empty.");
            osre_info(Tag, ap.showHelp());
            return false;
        }

        if (!AppBase::onCreate()) {
            osre_error(Tag, "Error while creating app.");
            return false;
        }

        AbstractDynamicLoader *dynLoader = PlatformInterface::getInstance()->getDynamicLoader();
        if (dynLoader == nullptr) {
            osre_error(Tag, "Invalid dyn-loader detected.");
            return false;
        }

        mModule = dynLoader->load(mDllName);
        if (mModule == nullptr) {
            osre_error(Tag, "Cannot load library " + mDllName + ".");
            return false;
        }

        createModuleFn func = (createModuleFn)dynLoader->loadFunction("createModule");
        if (func == nullptr) {
            osre_error(Tag, "Cannot find factory method");
            return false;
        } else {
            osre_info(Tag, "Loading " + mDllName + " successful.");
        }

        return true;
    }

    bool onDestroy() override {
        AbstractDynamicLoader *dynLoader = PlatformInterface::getInstance()->getDynamicLoader();
        dynLoader->removeLib(mDllName);

        return AppBase::onDestroy();
    }

    void onRender() override {

    }

    void onUpdate() override {

    }

private:
    String mDllName;
    LibHandle *mModule;
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
