#pragma once

#include <osre/RenderBackend/RenderPass.h>

namespace OSRE {
namespace RenderBackend {

class RenderPass2D {
public:
    RenderPass2D();
    ~RenderPass2D();
    static RenderPass *build(guid id);
};

inline RenderPass2D::RenderPass2D() {}

inline RenderPass2D::~RenderPass2D() {}

inline RenderPass *RenderPass2D::build(guid id) {
    RenderPass *pass = new RenderPass(id, nullptr);
    
    return pass;
}

} // namespace RenderBackend
} // namespace OSRE
