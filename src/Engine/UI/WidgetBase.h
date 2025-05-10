/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/osre_common.h"
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE::Ui {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Todo!
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT WidgetBase {
public:
    WidgetBase(const Rect2i &rect, WidgetBase *parent);
    virtual ~WidgetBase();
    virtual void update();
    virtual void render(RenderBackend::CanvasRenderer *renderer);
    void setDirty();
    void setClean();
    bool isDirty() const;
    void setRect(const Rect2i &rect);
    const Rect2i &getRect() const;

protected:
    virtual void onUpdate() = 0;
    virtual void onRender(RenderBackend::CanvasRenderer *renderer) = 0;

private:
    WidgetBase *mParent;
    bool mDirty;
    Rect2i mRect;
};

inline WidgetBase::WidgetBase(const Rect2i &rect, WidgetBase *parent) : mParent(parent), mDirty(true), mRect(rect) {
    // empty
}

inline WidgetBase::~WidgetBase() {
    // empty
}

inline void WidgetBase::update() {
    onUpdate();
}

inline void WidgetBase::render(RenderBackend::CanvasRenderer *renderer) {
    onRender(renderer);
}

inline void WidgetBase::setDirty() {
    mDirty = true;
}

inline void WidgetBase::setClean() {
    mDirty = false;
}

inline bool WidgetBase::isDirty() const {
    return mDirty;
}

inline void WidgetBase::setRect(const Rect2i &rect) {
    if (mRect == rect) {
        return;
    }

    mRect = rect;
    setDirty();
}

inline const Rect2i &WidgetBase::getRect() const {
    return mRect;
}

} // namespace OSRE::Ui
