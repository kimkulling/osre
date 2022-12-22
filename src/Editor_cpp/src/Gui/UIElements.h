/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Platform/Windows/MinWindows.h>

#include <Commctrl.h>

namespace OSRE {

namespace Platform {
    class Win32Window;
    class AbstractPlatformEventQueue;
}

namespace Editor {
    class OsreEdApp;
}

struct ProgressBar;
struct TreeView;

// OS-specific API
class UIElements {
public:
    static ProgressBar *createProgressBar(int id, HWND hWnd, const Rect2ui &dimension);
    static void updateProgressBar(ProgressBar *pb, ui32 step);
    static void deleteProgressBar(ProgressBar *pb);
    static void createMenues(Platform::Win32Window *w, Editor::OsreEdApp *app, Platform::AbstractPlatformEventQueue *queue);
    static TreeView *createTreeView(Platform::Win32Window *w);
};

struct Widget {
    Widget *mParent;
    cppcore::TArray<Widget*> mChildren;
    Rect2ui mRect;
};

struct Label : Widget {
    String mText;
};

struct PlatformData {
    HWND mHWnd;
};

///	@brief  The progress-bar data.
struct ProgressBar : Widget {
    ui32 mRange;
    ui32 mCurrent;

    PlatformData mPlatformData;
};

struct TreeItem {
    using TreeItemArray = ::cppcore::TArray<TreeItem *>;

    String mName;
    TreeItem *mParent;
    TreeItemArray mChildren;

    String generateName();
    TreeItem();
    void addChild(TreeItem *item);
    size_t numChildren() const;
    TreeItem *getParent() const;
    PlatformData mPlatformData;

};

struct TreeView : Widget {
    TreeItem *mRoot;

    PlatformData mPlatformData;
};

struct Style {
    Color4 FG;
    Color4 BG;
    ui32 DefaultFontSize;
};

} // namespace OSRE
