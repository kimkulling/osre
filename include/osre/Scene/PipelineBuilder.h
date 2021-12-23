#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {
    class Pipeline;
    class RenderPass;
    class RenderBackendService;
    }
namespace Scene {

namespace ShaderToken {

    static const char *const IncludeToken = "#include";

}

class PipelineBuilder {
public:
    PipelineBuilder(RenderBackend::RenderBackendService *rbService);
    ~PipelineBuilder();
    void clear();
    PipelineBuilder &create(const String &name);
    RenderBackend::Pipeline *getPipeline() const;
    PipelineBuilder &addPipelinePass(RenderBackend::RenderPass *pipelinePass);

private:
    RenderBackend::Pipeline *mPipeline;
    RenderBackend::RenderBackendService *mRbService;
};

} // namespace Scene
} // namespace OSRE
