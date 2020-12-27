#pragma once

#include "Modules/ModuleBase.h"

#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace Editor {

class EditorModule : public ModuleBase {
public:
    EditorModule(App::AppBase *parentApp);
    ~EditorModule() override;

protected:
    bool onLoad() override;
    bool onUnload() override;
    void onUpdate() override;
    void onRender() override;

private:
    RenderBackend::TransformMatrixBlock m_transformMatrix;
};

}
} // namespace OSRE
