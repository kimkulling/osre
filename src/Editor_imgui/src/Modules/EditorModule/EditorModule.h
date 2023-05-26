#pragma once

#include <osre/Modules/ModuleBase.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>

namespace OSRE {
namespace Editor {

//-------------------------------------------------------------------------------------------------
/// @ingroup    Editor
///
/// @brief  This module is used to ...
//-------------------------------------------------------------------------------------------------
class EditorModule : public Modules::ModuleBase {
public:
    EditorModule(App::AppBase *parentApp);
    ~EditorModule() override = default;

protected:
    bool onLoad() override;
    bool onUnload() override;
    void onUpdate() override;
    void onRender() override;

private:
    RenderBackend::TransformMatrixBlock mTransformMatrix;
};

} // namepspace Editor
} // namespace OSRE
