/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/TFunctor.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Common/Variant.h>

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
        Max
    };

    CPPCore::TArray<Color4> m_colors;
    RenderBackend::FontBase *m_font;
    i32 m_horizontalMargin;
    i32 m_verticalMargin;

    Style()
    : m_colors()
    , m_font(nullptr)
    , m_horizontalMargin( -1 )
    , m_verticalMargin( -1 ) {
        initClassicDesign();
    }
    
    ~Style() {
        // empty
    }

    void initClassicDesign() {
        // colors widgets
        m_colors.add(Color4(1.f, 1.f, 1.f, 1.f));
        m_colors.add(Color4(0.9f, 0.9f, 0.9f, 1.f));

        // colors headline
        m_colors.add(Color4(1.f, 1.f, 1.f, 1.f));
        m_colors.add(Color4(0.5f, 0.5f, 0.5f, 1.f));

        // colors panel
        m_colors.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
        m_colors.add( Color4( 0.7f, 0.7f, 0.7f, 1.f ) );

        // colors text
        m_colors.add(Color4(1.f, 1.f, 1.f, 1.f));

        // Margins
        m_horizontalMargin = 1;
        m_verticalMargin = 1;
    }

    bool operator == (const Style &rhs) const {
        for (ui32 i = 0; i < (ui32)ColorTable::Max; i++) {
            if (m_colors[i] != rhs.m_colors[i]) {
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
    size_t m_startIndex;
    size_t m_numIndices;
    RenderBackend::Texture *m_texture;

    UiRenderCmd();
    ~UiRenderCmd();

    OSRE_NON_COPYABLE(UiRenderCmd)
};

struct UiRenderCmdCache {
    using RenderCmdArray = CPPCore::TArray<UiRenderCmd*>;

    UiVertexCache m_vc;             ///< Will store all vertices
    UiIndexCache  m_ic;             ///< Will store all indices
    RenderCmdArray m_renderCmds;    ///< Will store all render commands
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

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT StyleProvider {
public:
    static Style &getCurrentStyle();
    static void setStyle( const Style &newStyle );

private:
    StyleProvider();
    ~StyleProvider();

private:
    static StyleProvider *s_instance;
    Style m_activeStyle;
};

} // Namespace UI
} // Namespace OSRE
