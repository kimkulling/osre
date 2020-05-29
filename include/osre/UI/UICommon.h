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

} // Namespace UI
} // Namespace OSRE
