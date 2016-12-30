#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
    namespace RenderBackend {
        struct Geometry;
    }

    namespace UI {
        struct Style;

        class UIRenderUtils {
        public:
            static RenderBackend::Geometry *createRectFromStyle( const RectUI &rect, const Style &style );
        };
    }
}
