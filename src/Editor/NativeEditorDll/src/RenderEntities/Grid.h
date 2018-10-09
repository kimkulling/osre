#pragma once

#include <osre/Scene/Node.h>

namespace OSRE {
namespace NativeEditor {

class Grid : public ::OSRE::Scene::Node {
public:
    Grid( Common::Ids &ids );
    ~Grid();
};

}
}
