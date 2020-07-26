#include "FontRenderer.h"

#include <osre/Scene/MeshBuilder.h>
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/RenderBackend/Pipeline.h>

#include <osre/UI/Widget.h>
#include <osre/Math/BaseMath.h>

namespace OSRE {
namespace UI {

using namespace OSRE::RenderBackend;
using namespace OSRE::Scene;

namespace {

static const c8 *Tag = "FontRenderer";

}

FontRenderer::FontTextEntry::FontTextEntry() :
        mText(),
        mMesh( nullptr ){
    // empty
}

static void insertTextEntry(ui32 id, Mesh *geo, const String &text, FontRenderer::TextBoxHashMap &textBoxes) {
    FontRenderer::FontTextEntry *entry = new FontRenderer::FontTextEntry;
    entry->mMesh = geo;
    entry->mText = text;
    textBoxes.insert(id, entry);
}

FontRenderer::FontRenderer() :
        m_transformMatrix(),
        mTextBoxes(),
        mTbArray() {
    // empty
}

FontRenderer::~FontRenderer() {
    clear();
}

void FontRenderer::AddRenderText(ui32 id, ui32 x, ui32 y, const String &text, RenderBackendService *rbSrv) {
    if (text.empty() || nullptr == rbSrv) {
        return;
    }

    f32 scale = 0.2f;
    glm::vec3 translate(0.f, 0.f, 0);
    m_transformMatrix.init();
    m_transformMatrix.m_model = glm::translate(m_transformMatrix.m_model, translate);
    m_transformMatrix.m_model = glm::scale(m_transformMatrix.m_model, glm::vec3(scale, scale, scale));
    m_transformMatrix.update();

    rbSrv->beginPass(PipelinePass::getPassNameById(UiPassId));
    rbSrv->beginRenderBatch("fontBatch");

    f32 xTrans(0), yTrans(0);
    UI::WidgetCoordMapping::mapPosToWorld(x, y, xTrans, yTrans);
    MeshBuilder meshBuilder;
    if (!mTextBoxes.hasKey(id)) {
        Mesh *mesh = meshBuilder.allocTextBox(xTrans, yTrans, scale, text, BufferAccessType::ReadWrite)
                             .getMesh();
        rbSrv->addMesh(mesh, 0);
        insertTextEntry(id, mesh, text, mTextBoxes);
        mesh->m_localMatrix = true;
        mesh->m_model = m_transformMatrix.m_model;
    } else {
        FontTextEntry *entry = nullptr;
        if (mTextBoxes.getValue(id, entry)) {
            if (nullptr != entry) {
                if (entry->mText != text) {
                    Mesh *mesh(nullptr);
                    if (text.size() > entry->mText.size()) {
                        meshBuilder.allocTextBox(xTrans, yTrans, 0.1f, text, BufferAccessType::ReadWrite);
                        mesh = meshBuilder.getMesh();
                        entry->mMesh = mesh;
                    } else {
                        MeshBuilder::updateTextBox(entry->mMesh, 0.1f, text);
                        mesh = entry->mMesh;
                    }
                    rbSrv->updateMesh(mesh);
                }
            }
        }
    }

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

void FontRenderer::clear() {
    if (mTextBoxes.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < mTbArray.size(); i++) {
        if (nullptr != mTbArray[i]) {
            delete mTbArray[i];
        }
    }
    mTextBoxes.clear();
    mTextBoxes.init(100);
    mTbArray.resize(0);
    mTextBoxes.init(100);
}

} // namespace UI
} // namespace OSRE
