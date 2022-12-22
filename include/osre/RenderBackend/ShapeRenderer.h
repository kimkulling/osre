#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {

class Mesh;
class RenderBackendService;
class Shader;

class ShapeRenderer {
public:
    ShapeRenderer(RenderBackendService *rbSrv);
    ~ShapeRenderer() = default;
    ShapeRenderer &setColor(glm::vec4 color);
    ShapeRenderer &setShader(Shader *shader);
    ShapeRenderer &addRect(f32 x, f32 y, f32 w, f32 h, bool filled);
    void commit();

private:
    RenderBackendService *mRbSrv;
    glm::vec4 mColor;
    Shader *mShader;
    enum DirtyMode {
        Inited =0, Created, Updated
    } mDirtyMode;
    Mesh *mMesh;
};

} // namespace RenderBackend
} // namespace OSRE
