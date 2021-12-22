/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/Windows/MinWindows.h>
#include <Commctrl.h>

namespace OSRE {

struct ProgressBar;
        
// OS-specific API
class UIElements {
public:
    static ProgressBar *createProgressBar(int id, HWND hWnd, const Rect2ui &dimension);
    static void updateProgressBar(ProgressBar *pb, ui32 step);
    static void deleteProgressBar(ProgressBar *pb);
};

struct Widget {
    Widget *mParent;
    CPPCore::TArray<Widget*> mChildren;
    Rect2ui mRect;
};

///	@brief  The progress-bar data.
struct ProgressBar : Widget {
    ui32 mRange;
    ui32 mCurrent;
    HWND mHWnd;
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
