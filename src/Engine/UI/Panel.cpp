#include "Panel.h"
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE::Ui {

using namespace ::OSRE::RenderBackend;

Panel::Panel(const Rect2i &rect, WidgetBase *parent) : WidgetBase(rect, parent) {
    // empty
}

Panel::~Panel() {
    // empty
}

void Panel::addWidget(WidgetBase *widget) {
    if (nullptr == widget) {
        return;
    }

    mWidgets.add(widget);
}

void Panel::onUpdate() {
}

void Panel::onRender(CanvasRenderer *renderer) {
    osre_assert(renderer != nullptr);
    if (isDirty()) {
        const Rect2i &r = getRect();
        renderer->drawRect(r.x1, r.y1, r.width, r.height, true);
        for (size_t i=0; i<mWidgets.size(); ++i) {
            mWidgets[i]->render(renderer);
        }
        setClean();
    }
}

}
