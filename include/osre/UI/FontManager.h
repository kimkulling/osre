#pragma once

#include <osre/Common/osre_common.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {

    namespace RenderBackend {
        class FontBase;
    }

    namespace UI {
        class OSRE_EXPORT FontManager {
        public:
            static FontManager *create();
            static void destroy();
            static FontManager *getInstance();
            RenderBackend::FontBase *getFontByName( const String &fontName ) const;
            bool loadFont( const String &name );

        private:
            FontManager();
            ~FontManager();

        private:
            static FontManager *s_instance;

            typedef CPPCore::THashMap<ui32, RenderBackend::FontBase*> FontHashMap;
            FontHashMap m_fontHashMap;
        };
    }
}