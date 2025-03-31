#include "WidgetBase.h"

namespace OSRE::Ui {

class OSRE_EXPORT Button : public WidgetBase {
public:
    Button(const String &label, const Rect2i &rect, WidgetBase *parent = nullptr);
    ~Button() override;
    void setLabel(const String &text);
    const String &getLabel() const;

protected:
    void onUpdate() override;
    void onRender(RenderBackend::CanvasRenderer *renderer) override;

    private:
        String mLabel;
};

inline void Button::setLabel(const String &text) {
    mLabel = text;
    setDirty();
}

inline const String &Button::getLabel() const {
    return mLabel;
}

}
