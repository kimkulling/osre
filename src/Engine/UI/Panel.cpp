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
#include "Panel.h"
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE::Ui {

using namespace ::OSRE::RenderBackend;

Panel::Panel(const Rect2i &rect, WidgetBase *parent) : WidgetBase(rect, parent) {
    // empty
}

Panel::~Panel() {
    // empty
}

void Panel::addWidget(WidgetBase *widget) {
    if (nullptr == widget) {
        return;
    }

    mWidgets.add(widget);
}

void Panel::onUpdate() {
}

void Panel::onRender(CanvasRenderer *renderer) {
    osre_assert(renderer != nullptr);
    if (isDirty()) {
        const Rect2i &r = getRect();
        renderer->drawRect(r.x1, r.y1, r.width, r.height, true);
        for (size_t i=0; i<mWidgets.size(); ++i) {
            mWidgets[i]->render(renderer);
        }
        setClean();
    }
}

}
