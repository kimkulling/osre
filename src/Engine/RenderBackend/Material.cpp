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
#include "Common/Logger.h"

namespace OSRE::RenderBackend {

DECL_OSRE_LOG_MODULE(Material)

Material::Material(const String &name, const IO::Uri &uri) :
        mName(name),
        mType(MaterialType::ShaderMaterial),
        mNumTextures(0),
        mTextures(nullptr),
        mShader(nullptr),
        mNumParameters(0),
        mParameters(nullptr),
        mShineness(0.0f),
        mShinenessStrength(0.0f),
        mUri(uri) {
    // empty
}


void Material::setMaterialType(MaterialType matType) {
    mType = matType;
}

Material::~Material() {
    clear();
}

void Material::clear() {
    delete [] mTextures;
    mNumTextures = 0;
    mTextures = nullptr;    
}

MaterialType Material::getMaterialType() const {
    return mType;
}

void Material::createShader(const String &name, ShaderSourceArray &shaders) {
    mShader = new Shader(name);
    for (ui32 i = 0; i < static_cast<ui32>(ShaderType::Count); ++i) {
        if (!shaders[i].empty()) {
            mShader->setSource(static_cast<ShaderType>(i), shaders[i]);
        }
    }
}

bool Material::createTextures(size_t numTextures) {
    if (numTextures == 0) {
        clear();
        osre_error(Tag, "Number of textures is zero");
        return false;
    }
    mNumTextures = numTextures;
    mTextures = new Texture *[numTextures];

    return true;
}

void Material::clearTextures() {
    delete mTextures;
    mTextures = nullptr;
    mNumTextures = 0;
}

void Material::setTextureStage(size_t textureIndex, Texture *tex)  {
    if (textureIndex >= mNumTextures) {
        return;
    }
    mTextures[textureIndex] = tex;
}

size_t Material::getNumTextures() const {
    return mNumTextures;
}

Texture *Material::getTextureStageAt(size_t textureIndex) const {
    if (textureIndex >= mNumTextures) {
        return nullptr;
    }

    return mTextures[textureIndex];
}

size_t Material::getNumParameter() const {
    return mNumParameters;
}

} // namespace OSRE::RenderBackend
