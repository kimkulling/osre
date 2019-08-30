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
#include <osre/UI/Widget.h>

namespace OSRE {

namespace Platform {
    class AbstractWindow;
}

namespace UI {


//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  The factory class is used to generated new 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT UiItemFactory {
public:
    static UiItemFactory *createInstance( Platform::AbstractWindow *surface );
    static void destroyInstance();
    static UiItemFactory *getInstance();
    Widget *create( WidgetType type, const String &uiName, Widget *parent );

private:
    UiItemFactory( Platform::AbstractWindow *surface );
    ~UiItemFactory();

private:
    static UiItemFactory *s_instance;

    Platform::AbstractWindow *m_surface;
};

}
}
