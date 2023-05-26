/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Modules/InspectorModule/InspectorModule.h"
#include <osre/App/AppBase.h>

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
//#include "resource.h"

namespace OSRE {
namespace Editor {

using namespace OSRE::App;
using namespace OSRE::Modules;

class InspectorView : public IModuleView {
public:
    InspectorView() :
            IModuleView("InspectorView"),
            mRoot(nullptr) {
        // empty
    }

    ~InspectorView() override = default;

protected:
    void onCreate(const Rect2ui&) override {}
    void onUpdate() override {}
    void onDestroy() override {}

private:
    HTREEITEM mRoot;
};

InspectorModule::InspectorModule(AppBase *parentApp) :
        ModuleBase("inspector.module", parentApp),
        mInspectorView(nullptr) {
    // empty
}

InspectorModule::~InspectorModule() {
    delete mInspectorView;
}

bool InspectorModule::onLoad() {
    mInspectorView = new InspectorView;
    
    return true;
}

bool InspectorModule::onUnload() {
    delete mInspectorView;
    mInspectorView = nullptr;
    
    return true;
}

void InspectorModule::onUpdate() {
}

void InspectorModule::onRender() {

}

} // namespace Editor
} // namespace OSRE
