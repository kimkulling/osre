#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Platform {

enum PluginType {
#ifdef OSRE_WINDOWS
    WindowsPlugin = 0,
    SDL2Plugin,
#else
    SDL2Plugin = 0,
#endif // OSRE_WINDOWS
    MaxPlugin
};

} // Namespace Platform
} // Namespace OSRE
