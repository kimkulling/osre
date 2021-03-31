#pragma once

namespace OSRE {
namespace Platform {
    class AbstractWindow;
}
namespace Editor {

class ProgressReporter {
public:
    ProgressReporter(Platform::AbstractWindow* window);
    ~ProgressReporter();
    void start();
    void stop();
    void update();

protected:
    Platform::AbstractWindow *mWindow;
};

}
} // namespace OSRE
