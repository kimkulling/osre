#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {

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
};

} // namespace OSRE
