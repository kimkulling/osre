#include <osre/RenderBackend/ShapeRenderer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

static Mesh *createMesh(const String &name) {
    Mesh *mesh = new Mesh(name, VertexType::ColorVertex, IndexType::UnsignedInt);

    return mesh;
}

static void setVertexColor(ColorVert *v, size_t numVertx, glm::vec4 col) {
    for (size_t i = 0; i < numVertx; ++i) {
        v[i].color0 = col;
    }
}

ShapeRenderer::ShapeRenderer(RenderBackendService *rbSrv) :
        mRbSrv(rbSrv), mColor(), mShader(nullptr), mDirtyMode(Inited), mMesh(nullptr) {
    // empty
}

ShapeRenderer::~ShapeRenderer() {
}

ShapeRenderer &ShapeRenderer::setColor(glm::vec4 color) {
    mColor = color;

    return *this;
}

ShapeRenderer &ShapeRenderer::setShader(Shader *shader) {
    mShader = shader;

    return *this;
}

ShapeRenderer &ShapeRenderer::addRect(f32 x, f32 y, f32 w, f32 h, bool filled) {
    if (mMesh == nullptr) {
        mMesh = createMesh("shape");
        mDirtyMode = Created;
    } else {
        mDirtyMode = Updated;
    }

    ColorVert v[4];
    v[0].position.x = x;
    v[0].position.y = y;
    v[1].position.x = x + w;
    v[1].position.y = y;
    v[2].position.x = x + w;
    v[2].position.y = y + h;
    v[3].position.x = x;
    v[3].position.y = y + h;
    ui32 indices[6] = { 0, 1, 2, 1, 2, 3 };
    setVertexColor(v, 4, mColor);
    mMesh->attachVertices(v, 4);
    mMesh->attachIndices(indices, 6);

    return *this;
}

void ShapeRenderer::commit() {
    if (mMesh == nullptr || mDirtyMode == Inited) {
        return;
    }

    if (mDirtyMode == Created) {
        mRbSrv->addMesh(mMesh, 0);
    } else {
        mRbSrv->updateMesh(mMesh);
    }
}

} // namespace RenderBackend
} // namespace OSRE
