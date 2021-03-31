#include "WMDialog.h"
#include <osre/Platform/PlatformOperations.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace UI {


WMDialog::WMDialog(const String &name ) :
        Object(name) {
}

WMDialog::~WMDialog() {

}

bool WMDialog::openFileOpenDialog(const String &title, const c8 *extensions, IO::Uri &location) {
    Platform::PlatformOperations::getFileOpenDialog(title, extensions, location);
    return location.isEmpty() ? true : false;
}

}
} // namespace OSRE