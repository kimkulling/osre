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

    Rect2ui r;
    rootWindow->getWindowsRect(r);

    mPanel = new Panel("log.panel", parent->getRootPanel());
    mPanel->setHeadline("Log");

    UI::Style &style = UI::StyleProvider::getCurrentStyle();
    mPanel->setRect(r.getX1() + style.HorizontalMargin, 
        r.getY1() + r.getHeight() / 3,
        r.getWidth() - 2 * style.HorizontalMargin, 
        r.getHeight() - (r.getY1() + r.getHeight() / 3) - 2 * style.VerticalMargin);

    return true;
}

bool LogModule::onUnload(OsreEdApp *parent) {
    return true;
}

void LogModule::onUpdate() {
    
}

} // namespace Editor
} // namespace OSRE
