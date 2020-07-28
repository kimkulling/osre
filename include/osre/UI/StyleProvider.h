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

#include <osre/UI/UICommon.h>

namespace OSRE {
namespace UI {

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
    FontBase *StyleFont;
    i32 HorizontalMargin;
    i32 VerticalMargin;
    bool HasBorder;

    Style();
    ~Style();
    void initClassicDesign();
    bool operator==(const Style &rhs) const;
    bool operator!=(const Style &rhs) const;
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
