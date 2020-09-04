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

LogModule::LogModule() :
        ModuleBase( "log.module" ),
        mPanel(nullptr){
}
                
LogModule::~LogModule() {
    mPanel = nullptr;
}

bool LogModule::onLoad(OsreEdApp *parent) {
    Platform::AbstractWindow *rootWindow = parent->getRootWindow();
    if (nullptr == rootWindow) {
        return true;
    }

    return true;
}

bool LogModule::onUnload(OsreEdApp *parent) {
    return true;
}

void LogModule::onUpdate() {
    
}

} // namespace Editor
} // namespace OSRE
