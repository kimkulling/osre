#include "ProgressReporter.h"

#include <osre/Platform/AbstractWindow.h>

namespace OSRE {
namespace Editor {

ProgressReporter::ProgressReporter(Platform::AbstractWindow *window) :
        mWindow(window) {

}

ProgressReporter::~ProgressReporter() {
    mWindow = nullptr;
}

void ProgressReporter::start() {
    if (nullptr != mWindow) {
        mWindow->setWindowsMouseCursor(Platform::DefaultMouseCursorType::WaitCursor);
    }
}

void ProgressReporter::stop() {
    if (nullptr != mWindow) {
        mWindow->setWindowsMouseCursor(Platform::DefaultMouseCursorType::ComonCursor);
    }
}

void ProgressReporter::update() {

}

} // namespace Editor
} // namespace OSRE 