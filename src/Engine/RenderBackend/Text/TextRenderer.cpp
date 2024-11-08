#include "RenderBackend/Text/TextRenderer.h"
#include "RenderBackend/FontService.h"

namespace OSRE {
namespace RenderBackend {

static constexpr c8 Tag[] = "TextRenderer";

/* static void createFontMeshes(DrawCmdArray &drawCmdArray, Font2MeshMap &font2MeshMap, MeshInfoArray &meshInfoArray) {
    if (hasTexts(drawCmdArray)) {
        for (size_t i = 0; i < drawCmdArray.size(); ++i) {
            const auto &dc = drawCmdArray[i];
            if (dc->UseFont == nullptr) {
                continue;
            }

            const String &keyName = dc->UseFont->Name;
            const String meshName = "text." + keyName;
            Material *matFont = MaterialBuilder::createTextMaterial(keyName);
            Mesh *fontMesh = new Mesh(meshName, VertexType::RenderVertex, IndexType::UnsignedShort);
            meshInfoArray.add({ fontMesh, PrimitiveType::TriangleList, 0, 0 });
            fontMesh->setMaterial(matFont);
            font2MeshMap[keyName] = fontMesh;
        }
    }
}*/
bool getMeshInfo(Mesh *mesh, MeshInfoArray &meshInfoArray, MeshInfo &info) {
    if (nullptr == mesh) {
        return false;
    }

    for (size_t i = 0; i < meshInfoArray.size(); ++i) {
        if (meshInfoArray[i].mMesh == mesh) {
            info = meshInfoArray[i];
            return true;
        }
    }

    return false;
}


        void renderFontMesh(const DrawCmd &dc, Font2MeshMap &font2MeshMap, MeshInfoArray &meshInfoArray) {
    MeshInfo info;
    const String &fontKey = dc.UseFont->Name;
    Mesh *textMesh = nullptr;
    Font2MeshMap::const_iterator it = font2MeshMap.find(fontKey);
    if (it != font2MeshMap.end()) {
        textMesh = it->second;
        if (textMesh == nullptr) {
            osre_debug(Tag, "Invalid font mesh detected.");
            return;
        } else {
            getMeshInfo(textMesh, meshInfoArray, info);
        }
    }

    const ui32 lastIndex = textMesh->getLastIndex();
    renumberIndices(dc, info.mNumVertices);

    textMesh->attachVertices(dc.Vertices, dc.NumVertices * sizeof(RenderVert));
    textMesh->attachIndices(dc.Indices, dc.NumIndices * sizeof(ui16));
    info.mPrim = dc.PrimType;
    textMesh->setLastIndex(lastIndex + static_cast<ui16>(dc.NumIndices));
    info.mNumVertices += dc.NumVertices;
    info.mNumIndices += dc.NumIndices;
}

TextRenderer::TextRenderer() :
        IRenderPath(), mFont2MeshMap(), mDefaultFont(nullptr) {
    // empty
}

TextRenderer::~TextRenderer() {

}

void TextRenderer::addText(i32 x, i32 y, Font *font, const String &text) {
    Font *usedFont = font;
    if (usedFont == nullptr) {
        usedFont = FontService::getDefaultFont();
    }

    Mesh *textMesh = nullptr;
    Font2MeshMap::const_iterator it = mFont2MeshMap.find(usedFont->Name);
    if (it == mFont2MeshMap.end()) {
        textMesh = new Mesh(usedFont->Name + ".mesh", VertexType::RenderVertex, IndexType::UnsignedShort);
        mFont2MeshMap[usedFont->Name] = textMesh;
    } else {
        textMesh = it->second;
    }


}

void TextRenderer::preRender(RenderBackendService* rbSrv) {

}

void TextRenderer::render(RenderBackendService* rbSrv) {

}

void TextRenderer::postRender(RenderBackendService* rbSrv) {

}

bool TextRenderer::onCreate() {
    mDefaultFont = FontService::getDefaultFont();
    
    return (mDefaultFont != nullptr);
}


} // namespace RenderBackend
} // namespace OSRE
