/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {

// Forward declarations
namespace RenderBackend {
    class FontBase;

    struct Texture;
}

namespace UI {

class Widget;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT FontManager {
public:
    static FontManager *create();
    static void destroy();
    static FontManager *getInstance();
    FontBase* create(const IO::Uri& font);
    RenderBackend::FontBase *getFontByName( const String &fontName ) const;
    void setDefaultFont(RenderBackend::FontBase* defaultFont);
    RenderBackend::FontBase *getDefaultFont() const;
    RenderBackend::Texture *getDefaultFontAsTexture() const;

    void registerForUpdates(Widget* widget);
    void unregisterFromeUpdates(Widget* widget);
    void unregisterAllFromUpdates();

protected:
    void notifyUpdate();

private:
    FontManager();
    ~FontManager();

private:
    static FontManager *s_instance;

    using FontHashMap = CPPCore::THashMap<ui32, RenderBackend::FontBase*>;
    using UpdateRegistryArray = CPPCore::TArray<Widget*>;

    FontHashMap m_fontHashMap;
    UpdateRegistryArray m_updateRegistry;
    RenderBackend::FontBase *m_defaultFont;
};

} // Namespace UI
} // Namespace OSRE
