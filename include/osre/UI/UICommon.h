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
#pragma once

#include <osre/Common/TFunctor.h>
#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Common/Variant.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace RenderBackend {
class FontBase;
}

namespace UI {

class Widget;
class Image;
class TextBase;
class Canvas;

struct UiFlags {
    static const ui32 Resizable = 0x001;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Style {
    enum class ColorTable {
        FGColorWidget = 0,
        BGColorWidget,
        FGColorPanel,
        BGColoPanel,
        FGColorHeadLine,
        BGColorHeadline,
        TextColor,
        BorderColor,
        Max
    };

    CPPCore::TArray<Color4> StyleColors;
    RenderBackend::FontBase *StyleFont;
    i32 HorizontalMargin;
    i32 VerticalMargin;
    bool HasBorder;

    Style() :
            StyleColors(),
            StyleFont(nullptr),
            HorizontalMargin(-1),
            VerticalMargin(-1),
            HasBorder(true) {
        initClassicDesign();
    }

    ~Style() {
        // empty
    }

    void initClassicDesign() {
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
        HorizontalMargin = 1;
        VerticalMargin = 1;

        HasBorder = true;
    }

    bool operator==(const Style &rhs) const {
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

    bool operator!=(const Style &rhs) const {
        return !(*this == rhs);
    }
};

using RenderBackend::UiIndexCache;
using RenderBackend::UiVertexCache;

struct UiRenderCmd {
    size_t m_startIndex;
    size_t m_numIndices;
    RenderBackend::Texture *m_texture;

    UiRenderCmd();
    ~UiRenderCmd();

    OSRE_NON_COPYABLE(UiRenderCmd)
};

struct UiRenderCmdCache {
    using RenderCmdArray = CPPCore::TArray<UiRenderCmd *>;

    UiVertexCache m_vc; ///< Will store all vertices
    UiIndexCache m_ic; ///< Will store all indices
    RenderCmdArray m_renderCmds; ///< Will store all render commands
};

/// This enum is used to describe the widget type.
enum class WidgetType {
    Button = 0,
    Text,
    Image,
    Panel,
    Canvas,
    NumWidgets,

    InvalidWidgetType
};

using UiFunctor = Common::Functor<void, ui32, void *>;

/// @brief  Description of a single
struct UiProperty {
    String m_name;
    CPPCore::Variant m_data;
};

enum class WidgetState {
    Pressed = 0,
    Released = 1,
    NumWidgetState,

    InvalidWidgetState
};

enum class LayoutPolicy {
    Auto = 0,
    Fixed,
    NumLayoutPolicies,

    InvalidLayoutPolicy
};

class Canvas;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT StyleProvider {
public:
    static void setUsingCanvas(Canvas *canvas);
    static Style &getCurrentStyle();
    static void setStyle(const Style &newStyle);

private:
    StyleProvider();
    ~StyleProvider();
    void notifyCanvas();

private:
    static StyleProvider *s_instance;
    Style m_activeStyle;
    Canvas *m_canvas;
};

} // Namespace UI
} // Namespace OSRE
