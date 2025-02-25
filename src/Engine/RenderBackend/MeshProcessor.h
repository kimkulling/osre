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

#include "Common/AbstractProcessor.h"
#include "RenderBackend/RenderCommon.h"
#include "Common/TAABB.h"
#include "App/TransformComponent.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MeshProcessor : public Common::AbstractProcessor {
public:
    MeshProcessor();
    ~MeshProcessor() = default;
    bool execute() override;
    void addMesh( RenderBackend::Mesh *geo );
    const Common::AABB &getAABB() const;

private:
    void handleMesh( RenderBackend::Mesh *mesh );

private:
    RenderBackend::MeshArray mMeshArray;
    Common::AABB mAabb;
    i32 mDirty;
};

} // Namespace RenderBackend
} // Namespace OSRE
