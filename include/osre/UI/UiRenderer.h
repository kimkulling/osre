#pragma once

namespace OSRE {
    namespace RenderBackend {
        class RenderBackendService;
    }
    namespace UI {
        class Screen;

        class UiRenderer {
        public:
            UiRenderer();
            ~UiRenderer();
            void render( UI::Screen *screen, RenderBackend::RenderBackendService *rbService );
        };
    }
}
