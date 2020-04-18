/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>
#include <osre/UI/UICommon.h>
#include <osre/ui/Canvas.h>

namespace OSRE {
namespace UI {

static const c8 *Tag = "StyleProvider";

StyleProvider *StyleProvider::s_instance = nullptr;

void StyleProvider::setUsingCanvas(Canvas *canvas) {
    if (s_instance->m_canvas != canvas) {
        s_instance->m_canvas = canvas;
        if (nullptr != s_instance->m_canvas) {
            s_instance->m_canvas->requestLayouting();
            s_instance->m_canvas->requestRedraw();
        }
    }
}

Style &StyleProvider::getCurrentStyle() {
    if (nullptr == s_instance) {
        s_instance = new StyleProvider;
    }

    return s_instance->m_activeStyle;
}

void StyleProvider::setStyle(const Style &newStyle) {
    if (nullptr == s_instance) {
        static_cast<void>(StyleProvider::getCurrentStyle());
    }

    if (nullptr == s_instance) {
        osre_debug(Tag, "Singleton instance is nullptr.");
        return;
    }

    if (newStyle != s_instance->m_activeStyle) {
        s_instance->m_activeStyle = newStyle;
    }
}

StyleProvider::StyleProvider() :
        m_activeStyle(),
        m_canvas(nullptr) {
    // empty
}

StyleProvider::~StyleProvider() {
    // empty
}

} // Namespace UI
} // Namespace OSRE
