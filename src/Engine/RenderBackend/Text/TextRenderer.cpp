#include "RenderBackend/Text/TextRenderer.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/FontService.h"

namespace OSRE {
namespace RenderBackend {

static constexpr c8 Tag[] = "TextRenderer";

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
    } else {
        osre_error(Tag, "Font key not found in font2MeshMap, stopping render.");
        return;
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
    if (rbSrv == nullptr) {
        return;
    }

    glm::mat4 m(1);
    rbSrv->setMatrix(MatrixType::Model, m);
    rbSrv->setMatrix(MatrixType::View, m);
    rbSrv->setMatrix(MatrixType::Projection, m);
}

void TextRenderer::render(RenderBackendService* rbSrv) {
    if (rbSrv == nullptr) {
        return;
    }

    MeshInfo info;
    for (auto &it : mFont2MeshMap) {
        Mesh *mesh = it.second;
        if (mesh != nullptr) {
            if (getMeshInfo(mesh, mMeshInfoArray, info)) {
                mesh->addPrimitiveGroup(info.mNumIndices, info.mPrim, 0);

                rbSrv->addMesh(mesh, 0);
            }
        }
    }
}

void TextRenderer::postRender(RenderBackendService* rbSrv) {

}

bool TextRenderer::onCreate() {
    mDefaultFont = FontService::getDefaultFont();
    
    return (mDefaultFont != nullptr);
}

} // namespace RenderBackend
} // namespace OSRE
