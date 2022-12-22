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
#include "Win32DisplayInterface.h"
#include <osre/Platform/Windows/MinWindows.h>

#include <shellscalingapi.h>
namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "Win32DisplayInterface";

struct MonitorInfo {
    int iIndex;
    HMONITOR hMonitor;
};

BOOL CALLBACK GetMonitorByIndex(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData) {
    MonitorInfo *info = (MonitorInfo *)dwData;
    if (--info->iIndex < 0) {
        info->hMonitor = hMonitor;
        return FALSE;
    }
    return TRUE;
}

static i32 monitorCount() {
    DISPLAY_DEVICE displayDevice;
    ::memset(&displayDevice, 0, sizeof(displayDevice));
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    i32 deviceNum = 0;
    while (EnumDisplayDevices(NULL, deviceNum, &displayDevice, 0)) {
        ++deviceNum;
    }

    return deviceNum; //signals an error
}

Win32DisplayInterface::~Win32DisplayInterface() {
    if (!mMonitorInfo.isEmpty()) {
        for (size_t mi = 0; mi < mMonitorInfo.size(); ++mi) {
            delete mMonitorInfo[mi];
        }
    }
}

i32 Win32DisplayInterface::getNumDisplays() {
    const i32 displayCount{ monitorCount() };

    if (0 == displayCount) {
        return 0;
    }

    mMonitorInfo.resize(displayCount);
    for (i32 i = 0; i < displayCount; ++i) {
        MonitorInfo *info = new MonitorInfo;
        info->iIndex = i;
        info->hMonitor = NULL;
        EnumDisplayMonitors(NULL, NULL, GetMonitorByIndex, (LPARAM)info);
        mMonitorInfo[i] = info;
    }

    return displayCount;
}

bool Win32DisplayInterface::getDisplayResolution(ui32 displayIndex, ui32 &width, ui32 &height) {
    width = height = 0;
    if (displayIndex >= mMonitorInfo.size()) {
        return false;
    }

    MONITORINFOEX monitorInfo = {};
    monitorInfo.cbSize = sizeof(MONITORINFOEX);
    if (FALSE == GetMonitorInfoA(mMonitorInfo[displayIndex]->hMonitor, &monitorInfo)) {
        return false;
    }

    width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

    return true;
}

bool Win32DisplayInterface::getDisplayDPI(ui32 displayIndex, DisplayDPIInfo *ddpiinfo) {
    if (nullptr == ddpiinfo) {
        return false;
    }
    if (displayIndex >= mMonitorInfo.size()) {
        return false;
    }

    ui32 dpiX, dpiY;
    if (FALSE == ::GetDpiForMonitor(mMonitorInfo[displayIndex]->hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) {
        return false;
    }

    ddpiinfo->ddpi = static_cast<f32>(::sqrt(dpiX * dpiX + dpiY * dpiY));
    ddpiinfo->hdpi = static_cast<f32>(dpiX);
    ddpiinfo->vdpi = static_cast<f32>(dpiY);

    return true;
}

} // namespace Platform
} // namespace OSRE
