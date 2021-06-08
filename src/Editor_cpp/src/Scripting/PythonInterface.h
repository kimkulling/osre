#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Editor {

class PythonInterface {
public:
    PythonInterface();
    ~PythonInterface();
    bool create();
    bool destroy();
    bool runScript();

private:
    bool mCreated;
};

} // Namespace Editor
} // namespace OSRE
