/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "UI/UiService.h"
#include "UI/Panel.h"
#include "Common/StringUtils.h"
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE::Ui {

IMPLEMENT_SINGLETON( ::OSRE::Ui::UiService )

using namespace OSRE::RenderBackend;
using namespace OSRE::Common;

UiService::UiService() : 
        AbstractService("Ui/UiService"), mCanvasRenderer(nullptr) {
    // empty
}

UiService::~UiService() {
    // empty
}

void UiService::setCanvasRenderer(CanvasRenderer *canvasRenderer) {
    mCanvasRenderer = canvasRenderer;
}

UiService *UiService::create() {
    UiService *uiService = new UiService;
    return uiService;
}

Panel *UiService::createPanel(const String &name, const Rect2i &rect, WidgetBase *parent) {
    if (name.empty()) {
        return nullptr;
    }
    
    Panel *panel  = new Panel(rect, parent);
    const HashId hashId = StringUtils::hashName(name);
    mPanelMap.insert(hashId, panel);

    return panel;
}

bool UiService::onOpen() {
    return true;
}

bool UiService::onClose() {
    mPanelMap.size();
    return true;
} 

bool UiService::onUpdate() {
    return true;
}

} // namespace OSRE::Ui
