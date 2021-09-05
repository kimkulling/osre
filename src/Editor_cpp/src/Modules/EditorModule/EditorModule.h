#pragma once

#include "Modules/ModuleBase.h"

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>

namespace OSRE {
namespace Editor {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief  
//-------------------------------------------------------------------------------------------------
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
    RenderBackend::TransformMatrixBlock mTransformMatrix;
};

}
} // namespace OSRE
