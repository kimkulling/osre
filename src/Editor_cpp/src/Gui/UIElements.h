#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {

struct DrawCmd {
    ui32 VertexOffset;
    ui32 IndexOffset;
    ui32 NumElements;
    ui32 TextureId;
};

struct UiVert {
    glm::vec3 pos;
    glm::vec4 col;
    glm::vec2 tex;
};
struct DrawList {
    Rect2ui Rect;
    CPPCore::TArray<DrawCmd*> DrawCmdBuffer;
    RenderBackend::TVertexCache<UiVert> VertexCache;
    RenderBackend::TIndexCache<ui16> IndexCache;

};

struct Widget {
    Widget *mParent;
    CPPCore::TArray<Widget *> mChildren;
    Rect2ui mRect;
};

struct Label : Widget {
    String mLabel;
};

struct Headline : Widget {
    Label mName;
};
struct Panel : Widget {
    Headline mHeadline;
};

struct Button : Widget {
    Label mLabel;
};

struct TreeViewItem {
    String Name;
    CPPCore::TArray<TreeViewItem *> Children;
};

struct TreeView : Widget {
    Label mLabel;
    TreeViewItem *mRoot;
};

struct Style {
    Color4 FG;
    Color4 BG;
    ui32 DefaultFontSize;
};

} // namespace OSRE
