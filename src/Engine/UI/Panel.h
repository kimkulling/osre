#pragma once

#include "WidgetBase.h"

#include <cppcore/Container/TArray.h>

namespace OSRE::Ui {

using WidgetArray = cppcore::TArray<WidgetBase*>;

class OSRE_EXPORT Panel : public WidgetBase {
public:
    Panel(const Rect2i &rect, WidgetBase *parent = nullptr);
    ~Panel() override;
    void addWidget(WidgetBase *widget);

protected:
    void onUpdate() override;
    void onRender(RenderBackend::CanvasRenderer *renderer) override;

private:
    WidgetArray mWidgets;
};

}
