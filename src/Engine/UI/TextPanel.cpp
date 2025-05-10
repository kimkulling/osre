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
#include "UI/TextPanel.h"
#include "Debugging/osre_debugging.h"

namespace OSRE::Ui {

using namespace OSRE::RenderBackend;

TextPanel::TextPanel(const Rect2i &rect, WidgetBase *parent) : WidgetBase(rect, parent) {
	// empty
}

TextPanel::~TextPanel() {
    // empty
}

void TextPanel::clear() {
    mStringArray.clear();
}

void TextPanel::addLine(const String &text) {
    mStringArray.add(text);
}

void TextPanel::onUpdate() {
}

void TextPanel::onRender(CanvasRenderer *renderer) {
    if (!isDirty()) {
        return;
    }

    auto *font = renderer->getActiveFont();
    int32_t x = getRect().getX1();
    int32_t y = getRect().getX1();
    for (size_t i = 0; i < mStringArray.size(); ++i) {
        renderer->drawText(x, y, font->Size, mStringArray[i].c_str());
        y += font->Size;
    }
    setClean();
}

} // namespace OSRE::Ui
