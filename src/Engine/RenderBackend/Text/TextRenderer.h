#pragma once

#include <RenderBackend/RenderCommon.h>
#include <RenderBackend/Mesh.h>

#include <map>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

using Font2MeshMap = std::map<String, Mesh *>;

struct MeshInfo {
    Mesh *mMesh;
    PrimitiveType mPrim;
    size_t mNumVertices;
    size_t mNumIndices;
};

using MeshInfoArray = cppcore::TArray<MeshInfo>;

struct Character {
    ui32       TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;      // Size of glyph
    glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
    ui32       Advance;   // Offset to advance to next glyph
};

class TextRenderer : public IRenderPath {
public:
    /// @brief
    TextRenderer();

    /// @brief
    ~TextRenderer() override;
    
    void addText(i32 x, i32 y, Font *font, const String &text);

    void preRender(RenderBackendService *rbSrv) override;

    /// @brief Will be called to render the frame.
    /// @param rbSrv    The render-backend service.
    void render(RenderBackendService *rbSrv) override;

    /// @brief Will be called after rendering.
    /// @param rbSrv    The render-backend service.
    void postRender(RenderBackendService *rbSrv) override;

protected:
    bool onCreate() override;

private:
    Font2MeshMap mFont2MeshMap;
    Font *mDefaultFont;
    MeshInfoArray mMeshInfoArray;
};

} // namespace RenderBackend
} // namespace OSRE
