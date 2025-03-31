#include "Button.h"
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE::Ui {

using namespace ::OSRE::RenderBackend;

Button::Button(const String &label, const Rect2i &rect, WidgetBase *parent) : WidgetBase(rect, parent), mLabel(label) {
    setDirty();
}

Button::~Button() {
    // empty
}

void Button::onUpdate() {
    //empty
}

void Button::onRender(CanvasRenderer *renderer) {
    osre_assert(renderer != nullptr);
    if (isDirty()) {
        const Rect2i &r = getRect();
        renderer->drawRect(r.x1, r.y1, r.width, r.height, true);
        if (!mLabel.empty()) {
            renderer->drawText(r.x1+1, r.y1+1, r.height-2, mLabel);
        }
        setClean();
    }
}

}