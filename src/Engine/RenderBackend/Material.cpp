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
#include "RenderBackend/Material.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE {
namespace RenderBackend {

Material::Material(const String &name, const IO::Uri &uri) :
        mName(name),
        mType(MaterialType::ShaderMaterial),
        mNumTextures(0),
        mTextures(nullptr),
        mShader(nullptr),
        mNumParameters(0),
        m_parameters(nullptr),
        mShineness(0.0f),
        mShinenessStrength(0.0f),
        mUri(uri) {
    // empty
}

void Material::setMaterialType(MaterialType matType) {
    mType = matType;
}

Material::~Material() {
    delete mShader;
    delete[] mTextures;
}

MaterialType Material::getMaterialType() const {
    return mType;
}

void Material::createShader(ShaderSourceArray &shaders) {
    mShader = new Shader;
    for (ui32 i = 0; i < MaxShaderTypes; ++i) {
        if (!shaders[i].empty()) {
            mShader->setSource(static_cast<ShaderType>(i), shaders[i]);
        }
    }
}

} // namespace RenderBackend
} // namespace OSRE
