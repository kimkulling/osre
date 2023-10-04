#pragma once

namespace OSRE {
namespace RenderBackend {

struct Vert2D {
    glm::vec2 pos;
    glm::vec3 color;
};

class RenderBackendService;

class CanvasRenderer {
public:
    CanvasRenderer(RenderBackendService *rbSrv);
    ~CanvasRenderer();
    void drawRect(i32 x, i32, y, i32 w, i32 h);
    void render();

private:
    RenderBackendService *mRbSrv;
};

} // namespace RenderBackend
} // namespace OSRE
