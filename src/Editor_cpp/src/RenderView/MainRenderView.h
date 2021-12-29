#pragma once

#include <osre/App/Entity.h>

namespace OSRE {
namespace Editor {

class MainRenderView {
public:
    MainRenderView();
    ~MainRenderView();

private:
    App::Entity *mEntity;
};

} // namespace Editor
} // namespace OSRE
