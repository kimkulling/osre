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
#include <osre/UI/StyleProvider.h>
#include <osre/UI/Canvas.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace UI {

StyleProvider *StyleProvider::s_instance = nullptr;

static const c8 *Tag = "StyleProvider";

Style::Style() :
        StyleColors(),
        StyleFont(nullptr),
        HorizontalMargin(-1),
        VerticalMargin(-1),
        HasBorder(true) {
    initClassicDesign();
}

Style::~Style() {
    // empty
}

void Style::initClassicDesign() {
    // colors widgets
    StyleColors.add(Color4(1.f, 1.f, 1.f, 1.f));
    StyleColors.add(Color4(0.9f, 0.9f, 0.9f, 1.f));

    // colors headline
    StyleColors.add(Color4(1.f, 1.f, 1.f, 1.f));
    StyleColors.add(Color4(0.5f, 0.5f, 0.5f, 1.f));

    // colors panel
    StyleColors.add(Color4(1.f, 1.f, 1.f, 1.f));
    StyleColors.add(Color4(0.7f, 0.7f, 0.7f, 1.f));

    // colors text
    StyleColors.add(Color4(1.f, 1.f, 1.f, 1.f));

    // border color
    StyleColors.add(Color4(1.f, 1.f, 1.f, 1.f));

    // Margins
    HorizontalMargin = 2;
    VerticalMargin = 2;

    HasBorder = true;
}

bool Style::operator==(const Style &rhs) const {
    for (ui32 i = 0; i < (ui32)ColorTable::Max; i++) {
        if (StyleColors[i] != rhs.StyleColors[i]) {
            return false;
        }

        if (StyleFont != rhs.StyleFont) {
            return false;
        }
    }

    return true;
}

bool Style::operator!=(const Style &rhs) const {
    return !(*this == rhs);
}

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

void StyleProvider::notifyCanvas() {
    if (nullptr != m_canvas) {
        m_canvas->requestLayouting();
        m_canvas->requestRedraw();
    }
}

} // Namespace UI
} // Namespace OSRE
