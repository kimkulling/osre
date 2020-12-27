#include "Modules/LogModule/LogModule.h"
#include "Modules/IModuleView.h"
#include "OsreEdApp.h"

#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>
#include <osre/UI/StyleProvider.h>
#include <osre/Platform/AbstractWindow.h>

namespace OSRE {
namespace Editor {

using namespace OSRE::UI;
using namespace OSRE::App;

class LogView : public IModuleView {
public:
    LogView() :
        IModuleView( "logview" ) {

    }

    ~LogView() override {

    }

protected:
    void onUpdate() override {

    }
};

LogModule::LogModule(AppBase *parentApp) :
        ModuleBase( "log.module", parentApp ),
        mPanel(nullptr){
}
                
LogModule::~LogModule() {
    mPanel = nullptr;
}

bool LogModule::onLoad() {
    AppBase *parentApp = getParentApp();
    Platform::AbstractWindow *rootWindow = parentApp->getRootWindow();
    if (nullptr == rootWindow) {
        return true;
    }

    return true;
}

bool LogModule::onUnload() {
    return true;
}

void LogModule::onUpdate() {
    
}

} // namespace Editor
} // namespace OSRE
