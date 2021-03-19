#pragma once

#include <osre/UI/Widget.h>

namespace OSRE {
namespace UI {

class WMDialog : public Widget {
public:
    WMDialog(const String &name, Widget *parent);
    ~WMDialog() override;
    static bool openFileOpenDialog(const c8 *extensions, IO::Uri &location);
};

} // namespace UI
} // namespace OSRE