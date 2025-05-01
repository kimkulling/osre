#pragma once

#include "UI/WidgetBase.h"

#include <cppcore/Container/TArray.h>

namespace OSRE::Ui {

class TextPanel : public WidgetBase {
public:
    using StringArray = cppcore::TArray<String>;

    TextPanel(const Rect2i &rect, WidgetBase *parent);
    ~TextPanel() override;
    void clear();
    void addLine(const String &text);

protected:
    void onUpdate() override;
    void onRender(RenderBackend::CanvasRenderer *renderer) override;

private:
    StringArray mStringArray;
};

} // namespace OSRE::Ui
