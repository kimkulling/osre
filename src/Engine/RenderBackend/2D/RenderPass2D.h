/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "RenderBackend/RenderPass.h"

namespace OSRE::RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the 2D render pass.
//-------------------------------------------------------------------------------------------------
class RenderPass2D {
public:
    /// @brief The class constructor.
    RenderPass2D() = default;

    /// @brief The class destructor.
    ~RenderPass2D() = default;

    /// @brief Builds a 2D render pass
    /// @param[in] id               The pass id
    /// @param[in] framebufferId    The framebuffer id
    static RenderPass *build(guid id, guid framebufferId);
};

inline RenderPass *RenderPass2D::build(guid id, guid framebufferId) {
    RenderPass *pass = new RenderPass(id, framebufferId, nullptr);

    return pass;
}

} // namespace OSRE::RenderBackend
