#include <osre/Scene/PipelineBuilder.h>
#include <osre/RenderBackend/Pipeline.h>

namespace OSRE {
namespace Scene {

PipelineBuilder::PipelineBuilder() :
        mPipeline(nullptr) {
    // empty
}

PipelineBuilder::~PipelineBuilder() {
    mPipeline = nullptr;
}

PipelineBuilder &PipelineBuilder::create(const String &name) {
    if (nullptr != mPipeline) {
        return *this;
    }

    mPipeline = new RenderBackend::Pipeline(name);
    
    return *this;
}

RenderBackend::Pipeline *PipelineBuilder::getPipeline() const {
    return mPipeline;
}

PipelineBuilder &PipelineBuilder::addPipelinePass( RenderBackend::RenderPass *pipelinePass ) {
    if (nullptr == pipelinePass) {
        return *this;    
    }

    mPipeline->addPass(pipelinePass);

    return *this;
}

} // namespace Scene
} // namespace OSRE
