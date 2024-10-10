/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "App/SceneCommon.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/RenderCommon.h"
#include "Common/TResourceCache.h"

namespace OSRE {
namespace RenderBackend {


//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class takes care of all default materials.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MaterialBuilder {
public:
    using MaterialFactory = Common::TResourceFactory<RenderBackend::Material>;
    using MaterialCache = Common::TResourceCache<MaterialFactory, RenderBackend::Material>;

    /// @brief Will create the material builder instance.
    static void create(GLSLVersion glslVersion);

    /// @brief Will destroy the material builder instance.
    static void destroy();
    
    /// @brief  Will create the build-in material for the given type of vertex.
    /// @param  type    The vertex type.
    /// @return The build-in material instance will be returned.
    static Material *createBuildinMaterial( RenderBackend::VertexType type );
        
    /// @brief  Will create the texture material instance.
    /// @param  matName      The name for the material.
    /// @param  texResArray  The array with all textures to use.
    /// @param  type         The vertex type.
    /// @return The created instance will be returned.
    static Material* createTexturedMaterial(const String& matName, RenderBackend::TextureResourceArray& texResArray, 
        RenderBackend::VertexType type );
    
    /// @brief  Will create the texture material instance with your own shader code.
    /// @param  matName      The name for the material.
    /// @param  texResArray  The array with all textures to use.
    /// @param  VsSrc        The vertex shader source.
    /// @param  FsSrc        The fragment shader source.
    /// @return The created instance will be returned.
    static Material* createTexturedMaterial(const String& matName, RenderBackend::TextureResourceArray& texResArray, 
        const String& VsSrc, const String& FsSrc);

    /// @brief  Will create the debug render text material.
    /// @return The instance of the material.
    static RenderBackend::Material *createDebugRenderTextMaterial();

    static Material *create2DMaterial();

private:
    /// @brief The default class constructor.
    MaterialBuilder() = default;

    ///	@brief The class destructor.
    ~MaterialBuilder() = default;

private:    
    struct Data {
        GLSLVersion mVersion;
        MaterialCache *mMaterialCache;

        ~Data() {
            delete mMaterialCache;
        }
    };

    static Data *sData;
};

} // Namespace RenderBackend
} // Namespace OSRE
