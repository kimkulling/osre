/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace RenderBackend {
    class FontBase;
}

namespace UI {

class Widget;
class Image;
class TextBase;
class Screen;

struct UiFlags {
    static const ui32 Resizable = 0x001;
};

/// @brief  Behavior-flags for the widgets
static const ui32 WidgetResizable = 1;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Style {
    enum class ColorTable {
        FGColorPanel = 0,
        BGColorPanel,
        FGColorWidget,
        BGColorWidget,
        TextColor,
        Max
    };

    CPPCore::TArray<Color4> m_properties;
    RenderBackend::FontBase *m_font;

    Style()
    : m_properties()
    , m_font(nullptr) {
        // color panel
        m_properties.add(Color4(1.f, 1.f, 1.f, 1.f));
        m_properties.add(Color4(0.9f, 0.9f, 0.9f, 1.f));

        // color button
        m_properties.add(Color4(1.f, 1.f, 1.f, 1.f));
        m_properties.add(Color4(0.5f, 0.5f, 0.5f, 1.f));

        // color text
        m_properties.add(Color4(1.f, 1.f, 1.f, 1.f));
    }

    bool operator == (const Style &rhs) const {
        for (ui32 i = 0; i < (ui32)ColorTable::Max; i++) {
            if (m_properties[i] != rhs.m_properties[i]) {
                return false;
            }
            if (m_font != rhs.m_font) {
                return false;
            }
        }
        return true;
    }

    bool operator != (const Style &rhs) const {
        return !(*this == rhs);
    }
};

using RenderBackend::UiVertexCache;
using RenderBackend::UiIndexCache;

struct UiRenderCmd {
    UiVertexCache            m_vc;  ///< Will store all vertices
    UiIndexCache             m_ic;  ///< Will store all indices
    RenderBackend::Material *m_mat; ///< Will store the material

    UiRenderCmd();
    ~UiRenderCmd();

    OSRE_NON_COPYABLE(UiRenderCmd)
};

using UiRenderCmdCache = CPPCore::TArray<UiRenderCmd*>;

} // Namespace UI
} // Namespace OSRE
