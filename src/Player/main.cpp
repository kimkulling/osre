/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AppBase.h>
#include <osre/App/ModuleBase.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/Debug.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractDynamicLoader.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::App;

static const c8 *Tag = "player";

typedef void*(__stdcall *createModuleFn)(void);

class PlayerApplication : public AppBase {
public:
    PlayerApplication(int argc, c8 *argv[]) :
            AppBase(argc, (const char **)argv),
            mDllName(),
            mBreakOnStartup(false),
            mModule(nullptr) {
        // empty
    }

    ~PlayerApplication() override {
        // empty
    }

protected:
    bool onCreate() override {
        const ArgumentParser &ap = AppBase::getArgumentParser();
        mBreakOnStartup = ap.hasArgument("break_on_startup");
        if (mBreakOnStartup) {
            osre_info(Tag, "Break");
            Debugging::debugBreak();
        }

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

        App::ModuleBase *module = (App::ModuleBase*)( * func)();
        if (module == nullptr) {
            osre_error(Tag, "Cannot create instance of modeule from " + mDllName);
            return false;
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
    bool mBreakOnStartup;
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
