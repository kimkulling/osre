#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {
    class Pipeline;
    class RenderPass;
}
namespace Scene {

namespace ShaderToken {

static const char *const IncludeToken = "#include";

}

class PipelineBuilder {
public:
    PipelineBuilder();
    ~PipelineBuilder();
    PipelineBuilder &create(const String &name);
    RenderBackend::Pipeline *getPipeline() const;
    PipelineBuilder &addPipelinePass(RenderBackend::RenderPass *pipelinePass);

private:
    RenderBackend::Pipeline *mPipeline;
};

} // namespace Scene
} // namespace OSRE
