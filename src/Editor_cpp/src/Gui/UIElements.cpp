#include "UIElements.h"
#include <osre/Scene/MeshBuilder.h>

namespace OSRE {

using namespace ::OSRE::RenderBackend;

void drawLabel(Label &label, Style &style, DrawList &drawCmds) {
    Scene::MeshBuilder mb;
    Rect2ui r = label.mRect;
    mb.allocTextBox(r.getX1(), r.getY1(), style.DefaultFontSize, label.mLabel, BufferAccessType::ReadWrite);
}

} // namespace OSRE