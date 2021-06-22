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
#pragma once

#include <osre/Common/Object.h>
#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Editor {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief
//-------------------------------------------------------------------------------------------------
class IModuleView : Common::Object {
public:
    virtual ~IModuleView();
    virtual void create(Rect2ui rect);
    virtual void update();
    virtual void destroy();

protected:
    IModuleView(const String &name);
    virtual void onCreate(Rect2ui rect);
    virtual void onUpdate();
    virtual void onDestroy();
};

inline IModuleView::IModuleView( const String &name ) :
        Object( name ) {
    // empty
}

inline IModuleView::~IModuleView() {
    // empty
}

inline void IModuleView::create( Rect2ui rect ) {
    onCreate(rect);
}

inline void IModuleView::update() {
    // empty
}

inline void IModuleView::destroy() {
    onDestroy();
}


inline void IModuleView::onCreate( Rect2ui rect ) {
    // empty
}

inline void IModuleView::onUpdate() {
    // empty
}

inline void IModuleView::onDestroy() {
    // empty
}

} // namespace Editor
} // namespace OSRE
