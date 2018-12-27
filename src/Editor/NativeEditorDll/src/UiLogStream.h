#pragma once

#include <osre/Common/Logger.h>

#include "EditorMain.h"

extern fnc_log_callback g_fnc_log_callback;

namespace OSRE {
namespace NativeEditor {

class UiLogStream : public ::OSRE::Common::AbstractLogStream {
public:
    UiLogStream() 
    : AbstractLogStream() {
        // empty
    }

    void write(const String &message) override {
        g_fnc_log_callback(1, message.c_str());
    }
};
 
}
}
