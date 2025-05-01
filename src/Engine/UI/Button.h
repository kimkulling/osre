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
#include "WidgetBase.h"

namespace OSRE::Ui {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Todo!
//-------------------------------------------------------------------------------------------------
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
    if (text == mLabel) {
        return;
    }

    mLabel = text;
    setDirty();
}

inline const String &Button::getLabel() const {
    return mLabel;
}

} // namespace OSRE::Ui
