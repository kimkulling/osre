#include <osre/Scene/PipelineBuilder.h>
#include <osre/RenderBackend/Pipeline.h>

namespace OSRE {
namespace Scene {

bool parseInclude(const String &line, String &includeFile) {
    String::size_type pos = line.find(ShaderToken::IncludeToken);
    if (pos == String::npos) {
        return false;
    }

    String::size_type start = line.find('"');
    String::size_type end = line.rfind('"');
    includeFile = line.substr(start + 1, end - start - 2);

    return !includeFile.empty();
}
    
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
