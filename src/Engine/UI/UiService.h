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

#include "Common/AbstractService.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {
    class CanvasRenderer;
}
}

namespace OSRE::Ui {
    
class Panel;
class WidgetBase;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Todo!
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT UiService : public Common::AbstractService {
public:
    DECLARE_SINGLETON(UiService)

public:
    /// @brief 
    UiService();

    /// @brief 
    ~UiService();
    
    /// @brief 
    /// @param canvasRenderer 
    void setCanvasRenderer(RenderBackend::CanvasRenderer *canvasRenderer);
    
    /// @brief  Will create a new instance.
    /// @return The new created instance.
    static UiService *create();

    /// @brief 
    /// @param name 
    /// @return 
    Panel *createPanel(const String &name, const Rect2i &rect, WidgetBase *parent = nullptr);

protected:
    bool onOpen() override;
    bool onClose() override;
    bool onUpdate() override;

private:    
    RenderBackend::CanvasRenderer *mCanvasRenderer;
    struct PanelEntry {
        String name;
        Panel *panel;
    };
    using PanelArray = cppcore::TArray<PanelEntry>;
    PanelArray mPanelArray;
};

} // namespace OSRE::Ui
