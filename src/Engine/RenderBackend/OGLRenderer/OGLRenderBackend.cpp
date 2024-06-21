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
#include "RenderBackend/OGLRenderer/OGLRenderBackend.h"
#include "RenderBackend/OGLRenderer/OGLCommon.h"
#include "RenderBackend/OGLRenderer/OGLEnum.h"
#include "RenderBackend/OGLRenderer/OGLShader.h"
#include "Common/Logger.h"
#include "Common/glm_common.h"
#include "Debugging/osre_debugging.h"
#include "Debugging/MeshDiagnostic.h"
#include "IO/Stream.h"
#include "IO/Uri.h"
#include "Platform/AbstractOGLRenderContext.h"
#include "Platform/AbstractTimer.h"
#include "Profiling/PerformanceCounterRegistry.h"
#include "RenderBackend/RenderStates.h"
#include "RenderBackend/Shader.h"

#include <cppcore/CPPCoreCommon.h>
#include <cppcore/Memory/MemUtils.h>

#include "stb_image.h"

#include <iostream>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Platform;
using namespace cppcore;

static constexpr c8 Tag[] = "OGLRenderBackend";
static constexpr ui32 NotInitedHandle = 9999999;

OGLRenderBackend::OGLRenderBackend() :
        mClearColor(0.3f, 0.3f, 0.3f, 1.0f),
        mRenderCtx(nullptr),
        mBuffers(),
        mActiveVB(NotInitedHandle),
        mActiveIB(NotInitedHandle),
        mVertexArrays(),
        mActiveVertexArray(OGLNotSetId),
        mShaders(),
        mTextures(),
        mFreeTexSlots(),
        m_texLookupMap(),
        mParameters(),
        mShaderInUse(nullptr),
        mFreeBufferSlots(),
        mPrimitives(),
        mFpState(nullptr),
        mFpsCounter(nullptr),
        mOglCapabilities(),
        mFrameFuffers(),
        mOGLDriverInfo() {
    mBindedTextures.resize((size_t)TextureStageType::NumTextureStageTypes);
    for (size_t i = 0; i < (size_t)TextureStageType::NumTextureStageTypes; ++i) {
        mBindedTextures[i] = nullptr;
    }
}

OGLRenderBackend::~OGLRenderBackend() {
    destroy();
}

void OGLRenderBackend::enumerateGPUCaps() {
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mOglCapabilities.mMaxAniso);
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mOglCapabilities.mContextMask);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &mOglCapabilities.mMax3DTextureSize);
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &mOglCapabilities.mMaxTextureUnits);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &mOglCapabilities.mMaxTextureImageUnits);
    glGetIntegerv(GL_MAX_TEXTURE_COORDS, &mOglCapabilities.mMaxTextureCoords);

    mOglCapabilities.mGLSLVersionAsStr = (c8*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    mOglCapabilities.mGLSLVersion = getGlslVersionFromeString(mOglCapabilities.mGLSLVersionAsStr);
}

void OGLRenderBackend::setClearColor(const Color4& clearColor) {
    mClearColor = clearColor;
}

void OGLRenderBackend::setMatrix(MatrixType type, const glm::mat4 &mat) {
    switch (type) {
        case MatrixType::Model:
            mMatrixBlock.mModel = mat;
            break;
        case MatrixType::View:
            mMatrixBlock.mView = mat;
            break;
        case MatrixType::Projection:
            mMatrixBlock.mProjection = mat;
            break;
        case MatrixType::Normal:
            mMatrixBlock.mNormal = mat;
            break;
        default:
            osre_debug(Tag, "Not supported enum " + osre_to_string((i32)type));
            break;
    }
    mMatrixBlock.update();
}

const glm::mat4 &OGLRenderBackend::getMatrix(MatrixType type) const {
    switch (type) {
        case MatrixType::Model:
            return mMatrixBlock.mModel;
        case MatrixType::View:
            return mMatrixBlock.mView;
        case MatrixType::Projection:
            return mMatrixBlock.mProjection;
        case MatrixType::Normal:
            return mMatrixBlock.mNormal;
        default:
            osre_debug(Tag, "Not supported enum " + osre_to_string((i32)type));
            break;
    }
    return mMatrixBlock.mModel;
}

void OGLRenderBackend::applyMatrix() {
    OGLParameter *model = getParameter("Model");
    if (nullptr == model) {
        UniformDataBlob *blob = UniformDataBlob::create(ParameterType::PT_Mat4, 1);
        ::memcpy(blob->m_data, mMatrixBlock.getModelPtr(), sizeof(glm::mat4));
        model = createParameter("Model", ParameterType::PT_Mat4, blob, 1);
    } else {
        memcpy(model->m_data->m_data, mMatrixBlock.getModelPtr(), sizeof(glm::mat4));
    }
    setParameter(model);

    OGLParameter *view = getParameter("View");
    if (nullptr == view) {
        UniformDataBlob *blob = UniformDataBlob::create(ParameterType::PT_Mat4, 1);
        ::memcpy(blob->m_data, mMatrixBlock.getViewPtr(), sizeof(glm::mat4));
        view = createParameter("View", ParameterType::PT_Mat4, blob, 1);
    } else {
        memcpy(view->m_data->m_data, mMatrixBlock.getViewPtr(), sizeof(glm::mat4));
    }
    setParameter(view);

    OGLParameter *projection = getParameter("Projection");
    if (nullptr == projection) {
        UniformDataBlob *blob = UniformDataBlob::create(ParameterType::PT_Mat4, 1);
        ::memcpy(blob->m_data, mMatrixBlock.getProjectionPtr(), sizeof(glm::mat4));
        projection = createParameter("Projection", ParameterType::PT_Mat4, blob, 1);
    } else {
        memcpy(projection->m_data->m_data, mMatrixBlock.getProjectionPtr(), sizeof(glm::mat4));
    }
    setParameter(projection);
}

bool OGLRenderBackend::create(AbstractOGLRenderContext *renderCtx) {
    setRenderContext(renderCtx);

    mFpState = new RenderStates;
    enumerateGPUCaps();

    // checking the supported GL version
    mOGLDriverInfo.mGLVendorString = (const c8 *)glGetString(GL_VENDOR);
    if (mOGLDriverInfo.mGLVendorString) {
        String vendor(mOGLDriverInfo.mGLVendorString);
        osre_info(Tag, vendor);
    }
    mOGLDriverInfo.mGLRendererString = (const c8 *)glGetString(GL_RENDERER);
    if (mOGLDriverInfo.mGLRendererString) {
        String renderer(mOGLDriverInfo.mGLRendererString);
        osre_info(Tag, renderer);
    }
    mOGLDriverInfo.mGLVersionString = (const c8 *)glGetString(GL_VERSION);
    if (mOGLDriverInfo.mGLVersionString) {
        String version(mOGLDriverInfo.mGLVersionString);
        osre_info(Tag, version);
    }
    const char *GLExtensions = (const c8*) glGetString(GL_EXTENSIONS);
    if (GLExtensions) {
        String extensions(GLExtensions);
        setExtensions(extensions);
    }
    // or better yet, use the GL4.x way to get the version number
    glGetIntegerv(GL_MAJOR_VERSION, &mOGLDriverInfo.mOpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &mOGLDriverInfo.mOpenGLVersion[1]);

    c8 *slv = (c8 *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    osre_info(Tag, "Supported GLSL language " + String(slv));

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
    glDisable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    return true;
}

bool OGLRenderBackend::destroy() {
    delete mFpState;
    mFpState = nullptr;

    releaseAllShaders();
    releaseAllTextures();
    releaseAllVertexArrays();
    releaseAllBuffers();
    releaseAllParameters();
    releaseAllPrimitiveGroups();

    delete mFpsCounter;
    mFpsCounter = nullptr;

    return true;
}

void OGLRenderBackend::setTimer(Platform::AbstractTimer *timer) {
    if (nullptr == mFpsCounter) {
        mFpsCounter = new Profiling::FPSCounter(timer);
    }
}

void OGLRenderBackend::setRenderContext(Platform::AbstractOGLRenderContext *renderCtx) {
    if (mRenderCtx == renderCtx) {
        return;
    }
        
    mRenderCtx = renderCtx;
    if (nullptr != mRenderCtx) {
        mRenderCtx->activate();
    }
}

void OGLRenderBackend::clearRenderTarget(const ClearState &clearState) {
    GLbitfield glTarget{ 0 };
    const ui32 clear{ clearState.m_state };
    if (clear & (int)ClearState::ClearBitType::ColorBit) {
        glTarget |= GL_COLOR_BUFFER_BIT;
    }
    if (clear & (int)ClearState::ClearBitType::DepthBit) {
        glTarget |= GL_DEPTH_BUFFER_BIT;
    }
    if (clear & (int)ClearState::ClearBitType::StencilBit) {
        glTarget |= GL_STENCIL_BUFFER_BIT; 
    }

    glClear(glTarget);
    glClearColor(mClearColor.m_r, mClearColor.m_g, mClearColor.m_b, mClearColor.m_a);
}

void OGLRenderBackend::setViewport(i32 x, i32 y, i32 w, i32 h) {
    glViewport(x, y, w, h);
}

OGLBuffer *OGLRenderBackend::createBuffer(BufferType type) {
    size_t handle(OGLNotSetId);
    GLuint bufferId(OGLNotSetId);
    glGenBuffers(1, &bufferId);
    OGLBuffer *buffer(nullptr);
    if (mFreeBufferSlots.isEmpty()) {
        buffer = new OGLBuffer;
        handle = mBuffers.size();
        mBuffers.add(buffer);
    } else {
        handle = mFreeBufferSlots.back();
        buffer = mBuffers[handle];
        mFreeBufferSlots.removeBack();
    }
    buffer->m_handle = handle;
    buffer->m_type = type;
    buffer->m_oglId = bufferId;
    buffer->m_size = 0;

    return buffer;
}

OGLBuffer *OGLRenderBackend::getBufferById(guid geoId) {
    OGLBuffer *buffer(nullptr);
    for (ui32 i = 0; i < mBuffers.size(); i++) {
        if (mBuffers[i]->m_geoId == geoId) {
            buffer = mBuffers[i];
            break;
        }
    }
    return buffer;
}

void OGLRenderBackend::bindBuffer(OGLBuffer *buffer) {
    if (nullptr == buffer) {
        osre_debug(Tag, "Pointer to buffer is nullptr");
        return;
    }

    GLenum target = OGLEnum::getGLBufferType(buffer->m_type);
    glBindBuffer(target, buffer->m_oglId);

    //CHECKOGLERRORSTATE();
}

void OGLRenderBackend::bindBuffer(ui32 handle) {
    OGLBuffer *buf(mBuffers[handle]);
    if (nullptr != buf) {
        bindBuffer(buf);
    }
}

void OGLRenderBackend::unbindBuffer(OGLBuffer *buffer) {
    if (nullptr == buffer) {
        osre_debug(Tag, "Pointer to buffer is nullptr");
        return;
    }
    mActiveVB = NotInitedHandle;
    mActiveIB = NotInitedHandle;
    GLenum target = OGLEnum::getGLBufferType(buffer->m_type);
    glBindBuffer(target, 0);

    CHECKOGLERRORSTATE();
}

void OGLRenderBackend::copyDataToBuffer(OGLBuffer *buffer, void *data, size_t size, BufferAccessType usage) {
    if (nullptr == buffer) {
        osre_debug(Tag, "Pointer to buffer is nullptr");
        return;
    }
    GLenum target = OGLEnum::getGLBufferType(buffer->m_type);
    glBufferData(target, size, data, OGLEnum::getGLBufferAccessType(usage));

    CHECKOGLERRORSTATE();
}

void OGLRenderBackend::releaseBuffer(OGLBuffer *buffer) {
    if (nullptr == buffer) {
        osre_debug(Tag, "Pointer to buffer instance is nullptr, skipped.");
        return;
    }

    if (buffer->m_oglId == OGLNotSetId) {
        osre_debug(Tag, "OGL-Id was already released, skipped.");
        return;
    }

    const size_t slot = buffer->m_handle;
    glDeleteBuffers(1, &buffer->m_oglId);
    buffer->m_handle = OGLNotSetId;
    buffer->m_type = BufferType::EmptyBuffer;
    buffer->m_oglId = OGLNotSetId;
    mFreeBufferSlots.add(slot);
}

void OGLRenderBackend::releaseAllBuffers() {
    for (ui32 i = 0; i < mBuffers.size(); ++i) {
        OGLBuffer *buffer = mBuffers[i];
        if (nullptr != buffer) {
            if (buffer->m_type != BufferType::EmptyBuffer) {
                releaseBuffer(buffer);
            }
        }
    }
    mBuffers.clear();
    mFreeBufferSlots.clear();
}

bool OGLRenderBackend::createVertexCompArray(const VertexLayout *layout, OGLShader *shader, VertAttribArray &attributes) {
    if (nullptr == layout) {
        osre_debug(Tag, "Pointer to layout instance is nullptr");
        return false;
    }

    if (nullptr == shader) {
        osre_debug(Tag, "Pointer to shader instance is nullptr");
        return false;
    }

    size_t index(0);
    OGLVertexAttribute *attribute(nullptr);
    for (ui32 i = 0; i < layout->numComponents(); i++) {
        VertComponent &comp(layout->getAt(i));
        attribute = new OGLVertexAttribute;
        attribute->m_pAttributeName = getVertCompName(comp.m_attrib).c_str();
        attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
        attribute->m_size = OGLEnum::getOGLSizeForFormat(comp.m_format);
        attribute->m_type = OGLEnum::getOGLTypeForFormat(comp.m_format);
        attribute->m_ptr = (GLvoid *)index;
        attributes.add(attribute);
        index += attribute->m_size;
    }

    return true;
}

bool OGLRenderBackend::createVertexCompArray(VertexType type, OGLShader *shader, VertAttribArray &attributes) {
    if (nullptr == shader) {
        osre_debug(Tag, "Pointer to shader instance is nullptr");
        return false;
    }

    OGLVertexAttribute *attribute(nullptr);
    switch (type) {
        case VertexType::ColorVertex:
            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Position).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = 0;
            attributes.add(attribute);

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Normal).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = (const GLvoid *)offsetof(ColorVert, normal);
            attributes.add(attribute);

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Color0).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = (const GLvoid *)offsetof(ColorVert, color0);
            attributes.add(attribute);
            break;

        case VertexType::RenderVertex:
            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Position).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = 0;
            attributes.add(attribute);

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Normal).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = (const GLvoid *)offsetof(RenderVert, normal);
            attributes.add(attribute);

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::Color0).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = (const GLvoid *)offsetof(RenderVert, color0);
            attributes.add(attribute);

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName(VertexAttribute::TexCoord0).c_str();
            attribute->m_index = shader->getAttributeLocation(attribute->m_pAttributeName);
            attribute->m_size = 2;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = (const GLvoid *)offsetof(RenderVert, tex0);
            attributes.add(attribute);
            break;

        default:
            break;
    }

    return true;
}

void OGLRenderBackend::releaseVertexCompArray(TArray<OGLVertexAttribute *> &attributes) {
    if (!attributes.isEmpty()) {
        for (ui32 i = 0; i < attributes.size(); ++i) {
            if (nullptr != attributes[i]) {
                delete attributes[i];
            }
        }
        attributes.clear();
    }
}

OGLVertexArray *OGLRenderBackend::createVertexArray() {
    OGLVertexArray *vertexArray = new OGLVertexArray;
    glGenVertexArrays(1, &vertexArray->m_id);
    const size_t id = mVertexArrays.size();
    mVertexArrays.add(vertexArray);
    vertexArray->m_slot = id;

    return vertexArray;
}

bool OGLRenderBackend::bindVertexLayout(OGLVertexArray *va, OGLShader *shader, size_t stride, GLint loc,
        OGLVertexAttribute *attrib) {
    if (nullptr == va || nullptr == shader || nullptr == attrib) {
        return false;
    }

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, (GLint)attrib->m_size,
            attrib->m_type,
            GL_FALSE,
            (GLsizei)stride,
            attrib->m_ptr);

    return true;
}

bool OGLRenderBackend::bindVertexLayout(OGLVertexArray *va, OGLShader *shader, size_t stride,
        const TArray<OGLVertexAttribute *> &attributes) {
    if (nullptr == va || nullptr == shader) {
        return false;
    }

    for (ui32 i = 0; i < attributes.size(); ++i) {
        const c8 *attribName = attributes[i]->m_pAttributeName;
        if (nullptr == attribName) {
            continue;
        }

        const GLint loc = shader->getAttributeLocation(attribName);
        if (-1 != loc) {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, (GLint)attributes[i]->m_size,
                    attributes[i]->m_type,
                    GL_FALSE,
                    (GLsizei)stride,
                    attributes[i]->m_ptr);
        }
    }

    return true;
}

void OGLRenderBackend::destroyVertexArray(OGLVertexArray *vertexArray) {
    if (nullptr == vertexArray) {
        return;
    }

    glDeleteVertexArrays(1, &vertexArray->m_id);
    vertexArray->m_id = NotInitedHandle;
}

OGLVertexArray *OGLRenderBackend::getVertexArraybyId(ui32 id) const {
    OGLVertexArray *va(nullptr);
    if (id < mVertexArrays.size()) {
        va = mVertexArrays[id];
    }

    return va;
}

void OGLRenderBackend::bindVertexArray(OGLVertexArray *vertexArray) {
    if (nullptr == vertexArray) {
        osre_debug(Tag, "Pointer to vertex array is nullptr");
        return;
    }

    if ((mActiveVertexArray == OGLNotSetId) || (mActiveVertexArray != vertexArray->m_id)) {
        mActiveVertexArray = vertexArray->m_id;
        glBindVertexArray(mActiveVertexArray);
        CHECKOGLERRORSTATE();
    }
}

void OGLRenderBackend::unbindVertexArray() {
    glBindVertexArray(0);
    mActiveVertexArray = OGLNotSetId;
}

void OGLRenderBackend::releaseAllVertexArrays() {
    for (ui32 i = 0; i < mVertexArrays.size(); ++i) {
        destroyVertexArray(mVertexArrays[i]);
    }
    mVertexArrays.clear();
    mActiveVertexArray = OGLNotSetId;
}

static void loadShader(Shader *shaderInfo, OGLShader *oglShader, ShaderType type) {
    osre_assert(oglShader != nullptr);

    if (shaderInfo->hasSource(type)) {
        const bool result = oglShader->loadFromSource(type, shaderInfo->getSource(type));
        if (!result) {
            osre_error(Tag, "Error while compiling VertexShader.");
            return;
        }
    }
}

OGLShader *OGLRenderBackend::createShader(const String &name, Shader *shaderInfo) {
    if (name.empty()) {
        osre_debug(Tag, "Name for shader is nullptr");
        return nullptr;
    }

    OGLShader *oglShader = getShader(name);
    if (nullptr != oglShader) {
        return oglShader;
    }

    oglShader = new OGLShader(name);
    mShaders.add(oglShader);
    if (shaderInfo) {
        loadShader(shaderInfo, oglShader, ShaderType::SH_VertexShaderType);
        loadShader(shaderInfo, oglShader, ShaderType::SH_FragmentShaderType);
        loadShader(shaderInfo, oglShader, ShaderType::SH_GeometryShaderType);

        bool result = oglShader->createAndLink();
        if (!result) {
            osre_error(Tag, "Error while linking shader");
        }
    }

    return oglShader;
}

OGLShader *OGLRenderBackend::getShader(const String &name) {
    if (name.empty()) {
        return nullptr;
    }

    OGLShader *shader = nullptr;
    for (ui32 i = 0; i < mShaders.size(); ++i) {
        if (mShaders[i]->getName() == name) {
            shader = mShaders[i];
            break;
        }
    }

    return shader;
}

bool OGLRenderBackend::useShader(OGLShader *shader) {
    if (mShaderInUse == shader) {
        // shader already in use
        return true;
    }

    // unuse an older shader
    if (nullptr != mShaderInUse) {
        mShaderInUse->unuse();
    }

    // use new shader
    mShaderInUse = shader;
    if (nullptr != mShaderInUse) {
        mShaderInUse->use();
    }

    return true;
}

OGLShader *OGLRenderBackend::getActiveShader() const {
    return mShaderInUse;
}

bool OGLRenderBackend::releaseShader(OGLShader *shader) {
    if (nullptr != shader) {
        return false;
    }

    // look for the shader
    ui32 idx(0);
    bool found(false);
    for (ui32 i = 0; i < mShaders.size(); ++i) {
        if (mShaders[i] == shader) {
            found = true;
            idx = i;
            break;
        }
    }

    // remove shader from list
    if (found) {
        delete mShaders[idx];
        mShaders.remove(idx);
    }

    return found;
}

void OGLRenderBackend::releaseAllShaders() {
    for (ui32 i = 0; i < mShaders.size(); ++i) {
        if (nullptr != mShaders[i]) {
            if (mShaderInUse == mShaders[i]) {
                useShader(nullptr);
            }
            delete mShaders[i];
        }
    }
    mShaders.clear();
}

OGLTexture *OGLRenderBackend::createEmptyTexture(const String &name, TextureTargetType target, PixelFormatType format,
        ui32 width, ui32 height, ui32 channels) {
    if (name.empty()) {
        osre_debug(Tag, "Texture anme is empty.");
        return nullptr;
    }

    // lookup for texture
    OGLTexture *tex = findTexture(name);
    if (tex != nullptr) {
        return tex;
    }

    // get texture slot
    size_t slot = 0;
    if (mFreeTexSlots.isEmpty()) {
        slot = mTextures.size();
        tex = new OGLTexture;
        mTextures.add(tex);
    } else {
        slot = mFreeTexSlots.back();
        mFreeTexSlots.removeBack();
        tex = mTextures[slot];
    }
    tex->m_slot = slot;
    m_texLookupMap[name] = slot;

    GLuint textureId;
    glGenTextures(1, &textureId);
    tex->m_textureId = textureId;
    tex->m_name = name;
    tex->m_width = static_cast<ui32>(width);
    tex->m_height = static_cast<ui32>(height);
    tex->m_channels = static_cast<ui32>(channels);
    tex->m_format = OGLEnum::getGLTextureFormat(format);

    glActiveTexture(GL_TEXTURE0);
    tex->m_target = OGLEnum::getGLTextureTarget(target);
    glBindTexture(tex->m_target, textureId);

    glTexParameteri(tex->m_target, OGLEnum::getGLTextureEnum(TextureParameterName::TextureParamMinFilter), GL_LINEAR);
    glTexParameteri(tex->m_target, OGLEnum::getGLTextureEnum(TextureParameterName::TextureParamMagFilter), GL_LINEAR);
    glTexParameteri(tex->m_target, OGLEnum::getGLTextureEnum(TextureParameterName::TextureParamWrapS), GL_CLAMP);
    glTexParameteri(tex->m_target, OGLEnum::getGLTextureEnum(TextureParameterName::TextureParamWrapT), GL_CLAMP);

    glTexParameterf(tex->m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, mOglCapabilities.mMaxAniso);

    return tex;
}

static constexpr c8 DefaultTextureName[] = "default_tex";

OGLTexture *OGLRenderBackend::createDefaultTexture(TextureTargetType target, PixelFormatType pixelFormat, ui32 width, ui32 height) {
    OGLTexture *glTex = createEmptyTexture(DefaultTextureName, target, pixelFormat, width, height, 1);
    c8 *imageData = new c8[width * height];
    int value;
    size_t offset = 0;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            // Each cell is 8x8, value is 0 or 255 (black or white)
            value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
            imageData[offset] = (GLubyte)value;
            offset++;
        }
    }

    glTexImage2D(glTex->m_target, 0, GL_RGB, width, height, 0, OGLEnum::getGLTextureFormat(pixelFormat), GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(glTex->m_target);
    glTexParameterf(glTex->m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, mOglCapabilities.mMaxAniso);
    glBindTexture(glTex->m_target, 0);

    return glTex;
}

void OGLRenderBackend::updateTexture(OGLTexture *oglTextue, ui32 offsetX, ui32 offsetY, c8 *data, size_t size) {
    if (nullptr == oglTextue) {
        osre_error(Tag, "Pointer to texture is a nullptr.");
        return;
    }

    const ui32 diffX = oglTextue->m_width - offsetX;
    const ui32 diffY = oglTextue->m_height - offsetY;
    const ui32 subSize = diffX * diffY * oglTextue->m_channels;
    osre_validate(size < subSize, "Invalid size");
    glTexSubImage2D(oglTextue->m_target, 0, offsetX, offsetY, oglTextue->m_width,
            oglTextue->m_height, oglTextue->m_format, GL_UNSIGNED_BYTE, data);
}

OGLTexture *OGLRenderBackend::createTexture(const String &name, Texture *tex) {
    if (nullptr == tex) {
        return nullptr;
    }

    OGLTexture *glTex = findTexture(name);
    if (nullptr != glTex) {
        return glTex;
    }

    glTex = createEmptyTexture(name, tex->m_targetType, tex->mPixelFormat, tex->m_width, tex->m_height, tex->m_channels);
    glTexImage2D(glTex->m_target, 0, GL_RGB, tex->m_width, tex->m_height, 0, glTex->m_format, GL_UNSIGNED_BYTE, tex->m_data);
    glGenerateMipmap(glTex->m_target);
    glTexParameterf(glTex->m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, mOglCapabilities.mMaxAniso);
    glBindTexture(glTex->m_target, 0);

    return glTex;
}

OGLTexture *OGLRenderBackend::createTextureFromFile(const String &name, const IO::Uri &fileloc) {
    OGLTexture *tex = findTexture(name);
    if (nullptr == tex) {
        return tex;
    }

    // import the texture
    const String filename = fileloc.getAbsPath();
    i32 width = 0, height = 0, channels = 0;
    GLubyte *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        osre_debug(Tag, "Cannot load texture " + filename);
        return nullptr;
    }

    // swap the texture data
    // todo: must be done by my codec
    for (i32 j = 0; j * 2 < height; ++j) {
        i32 index1 = j * width * channels;
        i32 index2 = (height - 1 - j) * width * channels;
        for (i32 i = width * channels; i > 0; --i) {
            GLubyte temp = data[index1];
            data[index1] = data[index2];
            data[index2] = temp;
            ++index1;
            ++index2;
        }
    }

    // create texture and fill it
    tex = createEmptyTexture(name, TextureTargetType::Texture2D, PixelFormatType::R8G8B8, width, height, channels);
    glTexImage2D(tex->m_target, 0, GL_RGB, width, height, 0, tex->m_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(tex->m_target, 0);

    stbi_image_free(data);
    return tex;
}

OGLTexture *OGLRenderBackend::findTexture(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    std::map<String, size_t>::const_iterator it = m_texLookupMap.find(name);
    if (it == m_texLookupMap.end()) {
        return nullptr;
    }

    return mTextures[it->second];
}

bool OGLRenderBackend::bindTexture(OGLTexture *oglTexture, TextureStageType stageType) {
    if (nullptr == oglTexture) {
        return false;
    }

    GLenum glStageType = OGLEnum::getGLTextureStage(stageType);
    glActiveTexture(glStageType);
    glBindTexture(oglTexture->m_target, oglTexture->m_textureId);
    mBindedTextures[(size_t)stageType] = oglTexture;

    return true;
}

bool OGLRenderBackend::unbindTexture( TextureStageType stageType ) {
    const size_t index = (size_t)stageType;
    if (index >= mBindedTextures.size()) {
        return false;
    }

    if (nullptr != mBindedTextures[index]) {
        OGLTexture *oglTexture = mBindedTextures[index];
        glBindTexture(oglTexture->m_target, 0);
        mBindedTextures[index] = nullptr;
    }

    return true;
}

void OGLRenderBackend::releaseTexture(OGLTexture *oglTexture) {
    if (oglTexture == nullptr) {
        return;
    }

    if (nullptr == mTextures[oglTexture->m_slot]) {
        return;
    }

    glDeleteTextures(1, &oglTexture->m_textureId);
    oglTexture->m_textureId = OGLNotSetId;
    oglTexture->m_width = 0;
    oglTexture->m_height = 0;
    oglTexture->m_channels = 0;

    mFreeTexSlots.add(oglTexture->m_slot);

    std::map<String, size_t>::iterator it = m_texLookupMap.find(oglTexture->m_name);
    if (m_texLookupMap.end() != it) {
        it = m_texLookupMap.erase(it);
    }
    oglTexture->m_slot = 0;
}

void OGLRenderBackend::releaseAllTextures() {
    for (ui32 i = 0; i < mTextures.size(); ++i) {
        if (mTextures[i]->m_textureId != OGLNotSetId) {
            releaseTexture(mTextures[i]);
            delete mTextures[i];
        }
    }
    mFreeTexSlots.clear();
    mTextures.clear();
    m_texLookupMap.clear();
}

OGLParameter *OGLRenderBackend::createParameter(const String &name, ParameterType type,
        UniformDataBlob *blob, size_t numItems) {
    // Check if the parameter is already there
    OGLParameter *param = getParameter(name);
    if (nullptr != param) {
        return param;
    }

    // We need to create it
    param = new OGLParameter;
    param->m_name = name;
    param->m_type = type;
    param->m_loc = NoneLocation;
    param->m_numItems = numItems;
    param->m_data = UniformDataBlob::create(type, param->m_numItems);
    if (nullptr != blob) {
        if (0 != blob->m_size) {
            ::memcpy(param->m_data->getData(), blob->getData(), blob->m_size);
        }
    }
    mParameters.add(param);

    return param;
}

OGLParameter *OGLRenderBackend::getParameter(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    for (ui32 i = 0; i < mParameters.size(); ++i) {
        if (mParameters[i]->m_name == name) {
            return mParameters[i];
        }
    }

    return nullptr;
}

void OGLRenderBackend::setParameter(OGLParameter *param) {
    if (nullptr == param) {
        osre_debug(Tag, "Cannot set parameter, invalid param pointer.");
        return;
    }

    if (nullptr == mShaderInUse) {
        osre_debug(Tag, "Cannot set parameter, no shader in use.");
        return;
    }

    if (NoneLocation == param->m_loc) {
        param->m_loc = mShaderInUse->getUniformLocation(param->m_name);
        if (NoneLocation == param->m_loc) {
            osre_debug(Tag, "Cannot location for parameter " + param->m_name + " in shader " + mShaderInUse->getName() + ".");
            return;
        }
    }

    switch (param->m_type) {
        case ParameterType::PT_Int: {
            GLint data;
            ::memcpy(&data, param->m_data->getData(), sizeof(GLint));
            glUniform1i(param->m_loc, data);
        } break;

        case ParameterType::PT_IntArray: {
            glUniform1iv(param->m_loc, (GLsizei)param->m_numItems, (i32 *)param->m_data->getData());
        } break;

        case ParameterType::PT_Float: {
            GLfloat value;
            ::memcpy(&value, param->m_data->getData(), sizeof(GLfloat));
            glUniform1f(param->m_loc, value);
        } break;

        case ParameterType::PT_FloatArray: {
            glUniform1fv(param->m_loc, (GLsizei)param->m_numItems, (f32 *)param->m_data->getData());

        } break;

        case ParameterType::PT_Float2: {
            GLfloat value[2] = {};
            ::memcpy(&value[0], param->m_data->getData(), sizeof(GLfloat) * 2);
            glUniform2f(param->m_loc, value[0], value[1]);
        } break;

        case ParameterType::PT_Float2Array: {
            glUniform2fv(param->m_loc, (GLsizei)param->m_numItems, (f32 *)param->m_data->getData());
        } break;

        case ParameterType::PT_Float3: {
            GLfloat value[3] = {};
            ::memcpy(&value[0], param->m_data->getData(), sizeof(GLfloat) * 3);
            glUniform3f(param->m_loc, value[0], value[1], value[2]);
        } break;

        case ParameterType::PT_Float3Array: {
            glUniform3fv(param->m_loc, (GLsizei)param->m_numItems, (f32 *)param->m_data->getData());

        } break;

        case ParameterType::PT_Mat4: {
            glm::mat4 mat;
            ::memcpy(&mat, param->m_data->getData(), sizeof(glm::mat4));
            glUniformMatrix4fv(param->m_loc, 1, GL_FALSE, glm::value_ptr(mat));
        } break;

        case ParameterType::PT_Mat4Array: {
            glUniformMatrix4fv(param->m_loc, (GLsizei)param->m_numItems, GL_FALSE, (f32 *)param->m_data->getData());
        } break;

        default:
            break;
    }
    CHECKOGLERRORSTATE();
}

void OGLRenderBackend::releaseAllParameters() {
    ContainerClear(mParameters);
}

void OGLRenderBackend::setParameter(OGLParameter **param, size_t numParam) {
    if (nullptr == mShaderInUse) {
        return;
    }

    for (ui32 i = 0; i < numParam; ++i) {
        OGLParameter *currentParam = param[i];
        if (nullptr != currentParam) {
            setParameter(currentParam);
        }
    }
}

size_t OGLRenderBackend::addPrimitiveGroup(PrimitiveGroup *grp) {
    if (nullptr == grp) {
        osre_error(Tag, "Group pointer is nullptr");
        return NotInitedHandle;
    }

    OGLPrimGroup *oglGrp = new OGLPrimGroup;
    oglGrp->m_primitive = OGLEnum::getGLPrimitiveType(grp->m_primitive);
    oglGrp->m_indexType = OGLEnum::getGLIndexType(grp->m_indexType);
    oglGrp->m_startIndex = (ui32)grp->m_startIndex;
    oglGrp->m_numIndices = grp->m_numIndices;

    const size_t idx = mPrimitives.size();
    mPrimitives.add(oglGrp);

    return idx;
}

void OGLRenderBackend::releaseAllPrimitiveGroups() {
    ContainerClear(mPrimitives);
}

OGLFrameBuffer *OGLRenderBackend::createFrameBuffer(const String &name, ui32 width, ui32 height, 
        PixelFormatType pixelFormat, bool depthBuffer) {
    OGLFrameBuffer *oglFB = new OGLFrameBuffer(name.c_str(), width, height);
    glGenFramebuffers(1, &oglFB->m_bufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, oglFB->m_bufferId);

    glGenTextures(1, &oglFB->m_renderedTexture);
    glBindTexture(GL_TEXTURE_2D, oglFB->m_renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    GLenum glPixelFormat = OGLEnum::getGLTextureFormat(pixelFormat);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, glPixelFormat, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (depthBuffer) {
        glGenRenderbuffers(1, &oglFB->m_depthrenderbufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, oglFB->m_depthrenderbufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 
            oglFB->m_depthrenderbufferId);
    }

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oglFB->m_renderedTexture, 0);

    // Set the list of render buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glDeleteFramebuffers(1, &oglFB->m_bufferId);
        glDeleteTextures(1, &oglFB->m_renderedTexture);
        delete oglFB;
        oglFB = nullptr;
    }

    if (nullptr != oglFB) {
        mFrameFuffers.add(oglFB);
    }

    return oglFB;
}

void OGLRenderBackend::bindFrameBuffer(OGLFrameBuffer *oglFB) {
    if (nullptr == oglFB) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, oglFB->m_bufferId);
    glViewport(0, 0, oglFB->m_width, oglFB->m_height);
}

OGLFrameBuffer *OGLRenderBackend::getFrameBufferByName(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    for (ui32 i = 0; i < mFrameFuffers.size(); ++i) {
        if (0 == strncmp(name.c_str(), mFrameFuffers[i]->m_name, name.size())) {
            return mFrameFuffers[i];
        }
    }

    return nullptr;
}

void OGLRenderBackend::releaseFrameBuffer(OGLFrameBuffer *oglFB) {
    if (nullptr == oglFB) {
        return;
    }

    for (ui32 i = 0; i < mFrameFuffers.size(); ++i) {
        if (mFrameFuffers[i] == oglFB) {
            glDeleteFramebuffers(1, &oglFB->m_bufferId);
            glDeleteTextures(1, &oglFB->m_renderedTexture);
            mFrameFuffers.remove(i);
        }
    }
}

#if _MSC_VER > 1920 && !defined(__clang__)
#   pragma warning(push)
#   pragma warning(disable : 4312)
#endif

void OGLRenderBackend::render(size_t primpGrpIdx) {
    OGLPrimGroup *grp(mPrimitives[primpGrpIdx]);
    if (nullptr != grp) {
        glDrawElements(grp->m_primitive,
                (GLsizei)grp->m_numIndices,
                grp->m_indexType,
                (const GLvoid *)grp->m_startIndex);
    }
}

#if _MSC_VER > 1920 && !defined(__clang__)
#   pragma warning(pop)
#endif

void OGLRenderBackend::render(size_t primpGrpIdx, size_t numInstances) {
    OGLPrimGroup *grp(mPrimitives[primpGrpIdx]);
    if (nullptr != grp) {
        glDrawArraysInstanced(grp->m_primitive,
                grp->m_startIndex,
                (GLsizei)grp->m_numIndices,
                (GLsizei)numInstances);
    }
}

void OGLRenderBackend::renderFrame() {
    osre_assert(nullptr != mRenderCtx);

    mRenderCtx->update();
    if (nullptr != mFpsCounter) {
        const ui32 fps = mFpsCounter->getFPS();
        Profiling::PerformanceCounterRegistry::setCounter("fps", fps);
    }
}

void OGLRenderBackend::setFixedPipelineStates(const RenderStates &states) {
    osre_assert(nullptr != mFpState);

    if (mFpState->m_applied) {
        if (mFpState->isEqual(states.m_clearState, states.m_depthState, states.m_transformState, states.m_polygonState,
                    states.m_cullState, states.m_blendState, states.m_samplerState, states.m_stencilState)) {
            return;
        }
    }

    mFpState->m_polygonState = states.m_polygonState;
    mFpState->m_blendState = states.m_blendState;
    mFpState->m_cullState = states.m_cullState;
    mFpState->m_samplerState = states.m_samplerState;
    mFpState->m_stencilState = states.m_stencilState;

    if (mFpState->m_cullState.m_cullMode == CullState::CullMode::Off) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(OGLEnum::getOGLCullFace(mFpState->m_cullState.m_cullFace));
        glPolygonMode(OGLEnum::getOGLCullFace(mFpState->m_cullState.m_cullFace),
                OGLEnum::getOGLPolygonMode(mFpState->m_polygonState.m_polyMode));
        glFrontFace(OGLEnum::getOGLCullState(mFpState->m_cullState.m_cullMode));
    }

    if (mFpState->m_blendState.m_blendFunc == BlendState::BlendFunc::Off) {
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
    }
    mFpState->m_applied = true;
}

void OGLRenderBackend::setExtensions(const String &extensions) {
    mOGLDriverInfo.mExtensions = extensions;
}

const String &OGLRenderBackend::getExtensions() const {
    return mOGLDriverInfo.mExtensions;
}

} // Namespace RenderBackend
} // Namespace OSRE
