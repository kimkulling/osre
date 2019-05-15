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
#include <osre/UI/UiItemFactory.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/TextBase.h>
#include <osre/UI/Image.h>
#include <osre/UI/Panel.h>
#include <osre/UI/Screen.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Debugging;
using namespace ::OSRE::Platform;

UiItemFactory *UiItemFactory::s_instance = nullptr;

UiItemFactory *UiItemFactory::createInstance( AbstractWindow *surface ) {
    if ( nullptr == s_instance ) {
        s_instance = new UiItemFactory( surface );
    }

    return s_instance;
}

void UiItemFactory::destroyInstance() {
    if ( nullptr != s_instance ) {
        delete s_instance;
        s_instance = nullptr;
    }
}

UiItemFactory *UiItemFactory::getInstance() {
    return s_instance;
}

Widget *UiItemFactory::create( WidgetType type, const String &uiName, Widget *parent ) {
    Widget *item( nullptr );
    switch ( type ) {
        case WidgetType::Button:
            item = new ButtonBase( uiName, parent );
            break;
        case WidgetType::Text:
            item = new TextBase( uiName, parent );
            break;
        case WidgetType::Image:
            item = new Image( uiName, parent );
            break;
        case WidgetType::Panel:
            item = new Panel( uiName, 0, parent );
            break;
        case WidgetType::Screen: {
                const ui32 w( m_surface->getProperties()->m_width );
                const ui32 h( m_surface->getProperties()->m_height );
                item = new Screen( uiName, parent, 0, 0, w, h );
            }
            break;

        default:
            break;
    }

    return item;
}

UiItemFactory::UiItemFactory( AbstractWindow *surface )
: m_surface( surface ) {
    OSRE_ASSERT( nullptr != surface );
}

UiItemFactory::~UiItemFactory() {
    // empty
}

} // Namespace UI
} // Namespace OSRE
