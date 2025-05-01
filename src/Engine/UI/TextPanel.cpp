#include "UI/TextPanel.h"
#include "Debugging/osre_debugging.h"

namespace OSRE::Ui {

using namespace OSRE::RenderBackend;

TextPanel::TextPanel(const Rect2i &rect, WidgetBase *parent) : WidgetBase(rect, parent) {

}

TextPanel::~TextPanel() {
    // empty
}

void TextPanel::clear() {
    mStringArray.clear();
}

void TextPanel::addLine(const String &text) {
    mStringArray.add(text);
}

void TextPanel::onUpdate() {
    if (!isDirty()) {
        return;
    }
}

void onRender(CanvasRenderer *renderer) {
    osre_assert(renderer != nullptr);

    auto *font = renderer->getActiveFont();
    font->Size
}

}
