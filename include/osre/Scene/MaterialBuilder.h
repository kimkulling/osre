/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Scene/SceneCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Common/TResourceCache.h>

namespace OSRE {
namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MaterialBuilder {
public:
    /// @brief Will create the material builder instance.
    static void create();

    /// @brief Will destroy the material builder instance.
    static void destroy();
    
    /// @brief  Will create the build-in material for the given type of vertex.
    /// @param  type    The vertx type.
    /// @return The build-in material instance.
    static RenderBackend::Material *createBuildinMaterial( RenderBackend::VertexType type );
    static RenderBackend::Material *createBuildinUiMaterial();
    static RenderBackend::Material* createTexturedMaterial(const String& matName, RenderBackend::TextureResourceArray& texResArray, 
        RenderBackend::VertexType type );
    static RenderBackend::Material* createTexturedMaterial(const String& matName, RenderBackend::TextureResourceArray& texResArray, 
        const String& VsSrc, const String& FsSrc);

private:
    /// @brief The default class constructor.
    MaterialBuilder();

    ///	@brief The class destructor.
    ~MaterialBuilder();

private:
    using MaterialFactory = Common::TResourceFactory<RenderBackend::Material>;
    using MaterialCache = Common::TResourceCache<MaterialFactory, RenderBackend::Material>;
    static MaterialCache *s_materialCache;
};

} // Namespace Scene
} // Namespace OSRE
