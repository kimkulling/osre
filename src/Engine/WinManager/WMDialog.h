#pragma once

#include <osre/Common/Object.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace UI {

class WMDialog : public Common::Object {
public:
    WMDialog(const String &name);
    ~WMDialog() override;
    static bool openFileOpenDialog(const c8 *extensions, IO::Uri &location);
};

} // namespace UI
} // namespace OSRE