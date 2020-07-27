#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Scene/SceneCommon.h>

#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
    class Mesh;
}

namespace UI {

struct Glyph {
    f32 x;
    f32 y;
};

class FontRenderer {
public:
    struct FontTextEntry;

    using TextBoxHashMap = CPPCore::THashMap<ui32, FontTextEntry*>;
    using TextBoxArray = CPPCore::TArray<FontTextEntry*>;

    struct FontTextEntry {
        String mText;
        RenderBackend::Mesh *mMesh;

        FontTextEntry();
    };

    FontRenderer();
    ~FontRenderer();
    void AddRenderText(ui32 id, ui32 x, ui32 y, const String &text, RenderBackend::RenderBackendService *rbSrv);
    void clear();

private:
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    TextBoxHashMap mTextBoxes;
    TextBoxArray mTbArray;
};

} // namespace UI
} // namespace OSRE
