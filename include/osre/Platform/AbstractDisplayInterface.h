#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Platform {

struct DisplayDPIInfo {
    f32 ddpi;
    f32 hdpi;
    f32 vdpi;
};

class AbstractDisplayInterface {
public:
    virtual ~AbstractDisplayInterface();
    virtual i32 getNumDisplays() = 0;
    virtual bool getDisplayResolution(ui32 displayIndex, ui32 &width, ui32 &height) = 0;
    virtual bool getDisplayDPI(ui32 displayIndex, DisplayDPIInfo *ddpiinfo ) = 0;
};

inline AbstractDisplayInterface::~AbstractDisplayInterface() {
    // empty
}

} // namespace Platform
} // namespace OSRE
