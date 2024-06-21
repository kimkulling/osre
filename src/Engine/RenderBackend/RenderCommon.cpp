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
#include "RenderBackend/RenderCommon.h"
#include "App/AssetRegistry.h"
#include "Common/Ids.h"
#include "Common/Logger.h"
#include "IO/Uri.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/Shader.h"
#include "Common/glm_common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::cppcore;
using namespace ::OSRE::Common;
using namespace ::glm;

VertComponent VertexLayout::ErrorComp;

// The log tag for messages
static const c8 *Tag = "RenderCommon";

/// @brief  The corresponding names for vertex components in a vertex layout
static const String
        VertCompName[static_cast<ui32>(VertexAttribute::NumVertexAttrs)] = {
            "position",  ///< Position
            "normal",    ///< Normal
            "texcoord0", ///< TexCoord0
            "texcoord1", ///< TexCoord1
            "texcoord2", ///< TexCoord2
            "texcoord3", ///< TexCoord3
            "tangent",   ///< Tangent
            "binormal",  ///< Binormal
            "weights",   ///< Weights
            "indices",   ///< Indices
            "color0",    ///< Color0
            "color1",    ///< Color1
            "instance0", ///< Instance0
            "instance1", ///< Instance1
            "instance2", ///< Instance2
            "instance3"  ///< Instance3
        };

static const String ErrorCmpName = "Error";

// List of attributes for color vertices
static constexpr ui32 NumColorVertAttributes = 3;
static const String ColorVertAttributes[NumColorVertAttributes] = {
    "position", "normal", "color0"
};

ColorVert::ColorVert() :
        position(), normal(), color0(1, 1, 1) {
    // empty
}

size_t ColorVert::getNumAttributes() {
    return NumColorVertAttributes;
}

const String *ColorVert::getAttributes() {
    return ColorVertAttributes;
}

// List of attributes for render vertices
static constexpr ui32 NumRenderVertAttributes = 4;

static const String RenderVertAttributes[NumRenderVertAttributes] = {
    "position", 
    "normal", 
    "color0", 
    "texcoord0"
};

RenderVert::RenderVert() :
        position(),
        normal(),
        color0(1, 1, 1),
        tex0() {
    // empty
}

bool RenderVert::operator==(const RenderVert &rhs) const {
    return (position == rhs.position && normal == rhs.normal &&
            color0 == rhs.color0 && tex0 == rhs.tex0);
}

bool RenderVert::operator!=(const RenderVert &rhs) const {
    return !(*this == rhs);
}

size_t RenderVert::getNumAttributes() {
    return NumRenderVertAttributes;
}

const String *RenderVert::getAttributes() {
    return RenderVertAttributes;
}

const String &getVertCompName(VertexAttribute attrib) {
    if (attrib > VertexAttribute::Instance3  || attrib == VertexAttribute::InvalidVertexAttr) {
        return ErrorCmpName;
    }
    return VertCompName[static_cast<size_t>(attrib)];
}

const size_t NumUIVertAttributes = 2;

static const String UIVertAttributes[NumUIVertAttributes] = {
    "position", "color0"
};

size_t UIVert::getNumAttributes() {
    return NumUIVertAttributes;
}

const String *UIVert::getAttributes() {
    return UIVertAttributes;
}

VertComponent::VertComponent() :
        m_attrib(VertexAttribute::InvalidVertexAttr),
        m_format(VertexFormat::InvalidVertexFormat) {
    // empty
}

VertComponent::VertComponent(VertexAttribute attrib, VertexFormat format) :
        m_attrib(attrib),
        m_format(format) {
    // empty
}

VertexLayout::VertexLayout() :
        m_attributes(nullptr),
        m_components(),
        m_offsets(),
        m_currentOffset(0),
        m_sizeInBytes(0) {
    // empty
}

VertexLayout::~VertexLayout() {
    delete[] m_attributes;
}

void VertexLayout::clear() {
    if (!m_components.isEmpty()) {
        for (size_t i = 0; i < m_components.size(); ++i) {
            delete m_components[i];
        }
        m_components.clear();
    }

    m_offsets.clear();
    m_currentOffset = 0;
}

size_t VertexLayout::sizeInBytes() {
    if (0 == m_sizeInBytes) {
        for (size_t i = 0; i < m_components.size(); ++i) {
            const size_t compSizeInBytes = getVertexFormatSize(m_components[i]->m_format);
            m_sizeInBytes += compSizeInBytes;
        }
    }

    return m_sizeInBytes;
}

size_t VertexLayout::numComponents() const {
    return m_components.size();
}

VertexLayout &VertexLayout::add(VertComponent *comp) {
    if (nullptr == comp) {
        return *this;
    }

    m_components.add(comp);
    const size_t offset(getVertexFormatSize(comp->m_format));
    m_offsets.add(m_currentOffset);
    m_currentOffset += offset;

    return *this;
}

VertComponent &VertexLayout::getAt(size_t idx) const {
    if (idx >= m_components.size()) {
        return ErrorComp;
    }

    return *m_components[idx];
}

const String *VertexLayout::getAttributes() {
    if (m_components.isEmpty()) {
        return nullptr;
    }

    if (nullptr == m_attributes) {
        const size_t numAttributes(m_components.size());
        m_attributes = new String[numAttributes];
        for (size_t i = 0; i < m_components.size(); ++i) {
            m_attributes[i] =
                    VertCompName[static_cast<int>(m_components[i]->m_attrib)];
        }
    }

    return m_attributes;
}

BufferData::BufferDataAllocator BufferData::sBufferDataAllocator(256);

BufferData::BufferData() :
        m_type(BufferType::EmptyBuffer),
        m_buffer(),
        m_cap(0),
        m_access(BufferAccessType::ReadOnly) {
    // empty
}

BufferData *BufferData::alloc(BufferType type, size_t sizeInBytes, BufferAccessType access) {
    BufferData *buffer = sBufferDataAllocator.alloc();
    buffer->m_cap = sizeInBytes;
    buffer->m_access = access;
    buffer->m_type = type;
    buffer->m_buffer.resize(sizeInBytes);

    return buffer;
}

void BufferData::copyFrom(void *data, size_t size) {
    if (nullptr == data) {
        return;
    }
    if (size > m_cap) {
        osre_error(Tag, "Out of buffer error.");
        return;
    }

    ::memcpy(&m_buffer[0], data, size);
}

void BufferData::attach(const void *data, size_t size) {
    c8 *ptr = (c8 *)data;
    for (size_t i = 0; i < size; ++i) {
        m_buffer.add(*ptr);
        ++ptr;
    }
}

BufferType BufferData::getBufferType() const {
    return m_type;
}

BufferAccessType BufferData::getBufferAccessType() const {
    return m_access;
}

PrimitiveGroup::PrimitiveGroup() :
        m_primitive(PrimitiveType::LineList), m_startIndex(0), m_numIndices(0), m_indexType(IndexType::UnsignedShort) {
    // empty
}

void PrimitiveGroup::init(IndexType indexType, size_t numPrimitives, PrimitiveType primType, size_t startIdx) {
    m_indexType = indexType;
    m_numIndices = numPrimitives;
    m_primitive = primType;
    m_startIndex = startIdx;
}

Texture::Texture() :
        m_textureName(""),
        m_loc(),
        m_targetType(TextureTargetType::Texture2D),
        mPixelFormat(PixelFormatType::R8G8B8),
        m_size(0),
        m_data(nullptr),
        m_width(0),
        m_height(0),
        m_channels(0),
        m_texHandle() {
    // empty
}

Texture::~Texture() {
    delete[] m_data;
}

size_t TextureLoader::load(const IO::Uri &uri, Texture *tex) {
    if (nullptr == tex) {
        return 0;
    }

    const String &filename = uri.getAbsPath();
    if (filename.find("$default") != String::npos) {
        tex = TextureLoader::getDefaultTexture();
    }

    String root = App::AssetRegistry::getPath("media");
    String path = App::AssetRegistry::resolvePathFromUri(uri);

    i32 width = 0, height = 0, channels = 0;
    
    tex->m_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (nullptr == tex->m_data) {
        osre_debug(Tag, "Cannot load texture " + filename);
        return 0;
    }
    tex->m_width = width;
    tex->m_height = height;
    tex->m_channels = channels;

    // swap the texture data
    for (i32 j = 0; j * 2 < height; ++j) {
        i32 index1 = j * width * channels;
        i32 index2 = (height - 1 - j) * width * channels;
        for (i32 i = width * channels; i > 0; --i) {
            uc8 temp = tex->m_data[index1];
            tex->m_data[index1] = tex->m_data[index2];
            tex->m_data[index2] = temp;
            ++index1;
            ++index2;
        }
    }

    const size_t size = width * height * channels;

    return size;
}

static Texture *DefaultTexture = nullptr;

RenderBackend::Texture *TextureLoader::getDefaultTexture() {
    if (nullptr == DefaultTexture) {
        DefaultTexture = new Texture;
    }

    DefaultTexture->m_channels = 3;
    DefaultTexture->m_width = 1;
    DefaultTexture->m_height = 1;
    DefaultTexture->m_targetType = TextureTargetType::Texture2D;
    DefaultTexture->m_data = new uc8[4];
    DefaultTexture->m_data[0] = 255;
    DefaultTexture->m_data[1] = 255;
    DefaultTexture->m_data[2] = 255;
    DefaultTexture->m_data[3] = 255;
    DefaultTexture->m_textureName = "default";

    return DefaultTexture;
}

bool TextureLoader::unload(Texture *tex) {
    if (nullptr == tex) {
        return false;
    }

    stbi_image_free(tex->m_data);
    tex->m_data = nullptr;
    tex->m_width = 0;
    tex->m_height = 0;
    tex->m_channels = 0;

    return true;
}

TextureResource::TextureResource(const String &name, const IO::Uri &uri) :
        TResource(name, uri),
        m_targetType(TextureTargetType::Texture2D),
        m_stage(TextureStageType::TextureStage0) {
    // empty
}

void TextureResource::setTargetType(TextureTargetType targetType) {
    m_targetType = targetType;
}

TextureTargetType TextureResource::getTargetType() const {
    return m_targetType;
}

void TextureResource::setTextureStage(TextureStageType stage) {
    m_stage = stage;
}

TextureStageType TextureResource::setTextureStage() const {
    return m_stage;
}

ResourceState TextureResource::onLoad(const IO::Uri &uri, TextureLoader &loader) {
    if (getState() == ResourceState::Loaded) {
        return getState();
    }

    Texture *tex = create();
    if (nullptr == tex) {
        return ResourceState::Error;
    }

    tex->m_textureName = getName();
    if (tex->m_textureName.find("$default") != String::npos) {
        tex = TextureLoader::getDefaultTexture();
        setState(ResourceState::Loaded);
        return getState();
    }

    getStats().m_memory = loader.load(uri, tex);
    tex->m_targetType = m_targetType;
    if (0 == getStats().m_memory) {
        setState(ResourceState::Error);
        osre_debug(Tag, "Cannot load texture " + uri.getAbsPath());
        return getState();
    }

    setState(ResourceState::Loaded);

    return getState();
}

ResourceState TextureResource::onUnload(TextureLoader &loader) {
    if (getState() == ResourceState::Unloaded) {
        return getState();
    }

    loader.unload(get());
    getStats().m_memory = 0;
    setState(ResourceState::Unloaded);

    return getState();
}

TransformState::TransformState() :
        m_translate(1.0f),
        m_scale(1.0f),
        m_rotation(1.0f) {
    // empty
}

TransformState::~TransformState() {
    // empty
}

void TransformState::setTranslation(f32 x, f32 y, f32 z) {
    m_translate.x = x;
    m_translate.y = y;
    m_translate.z = z;
}

void TransformState::setScale(f32 sx, f32 sy, f32 sz) {
    m_scale.x = sx;
    m_scale.y = sy;
    m_scale.z = sz;
}

bool TransformState::operator==(const TransformState &rhs) const {
    return (m_translate == rhs.m_translate && m_scale == rhs.m_scale &&
            m_rotation == rhs.m_rotation);
}

bool TransformState::operator!=(const TransformState &rhs) const {
    return !(*this == rhs);
}

void TransformState::toMatrix(mat4 &m) const {
    m *= glm::translate(m, m_translate);
    m *= glm::scale(m, m_scale);
    m *= m_rotation;
}

Viewport::Viewport() :
        m_x(-1), m_y(-1), m_w(-1), m_h(-1) {
    // empty
}

Viewport::Viewport(i32 x, i32 y, i32 w, i32 h) :
        m_x(x), m_y(y), m_w(w), m_h(h) {
    // empty
}

Viewport::Viewport(const Viewport &rhs) :
        m_x(rhs.m_x), m_y(rhs.m_y), m_w(rhs.m_w), m_h(rhs.m_h) {
    // empty
}
        
bool Viewport::operator==(const Viewport &rhs) const {
    return (m_x == rhs.m_x && m_y == rhs.m_y && m_w == rhs.m_w && m_h == rhs.m_h);
}

bool Viewport::operator!=(const Viewport &rhs) const {
    return !(*this == rhs);
}

Light::Light() :
        m_position(0.0f, 0.0f, 0.0f, 1.0f),
        m_specular(1.0f, 1.0f, 1.0f),
        m_diffuse(1.0f, 1.0f, 1.0f),
        m_ambient(1.0f, 1.0f, 1.0f),
        m_direction(0.0f, 0.0f, 1.0f, 1.0f),
        m_specularExp(1.0f),
        mRadius(1.0f),
        m_type(LightType::InvalidLightType) {
    // empty
}

Light::~Light() {
    // empty
}

MeshEntry *RenderBatchData::getMeshEntryByName(const c8 *name) {
    if (nullptr == name) {
        return nullptr;
    }

    for (auto &i : m_meshArray) {
        for (ui32 j = 0; j < i->mMeshArray.size(); ++j) {
            if (i->mMeshArray[j]->getName() == name) {
                return i;
            }
        }
    }

    return nullptr;
}

UniformVar *RenderBatchData::getVarByName(const c8 *name) {
    if (nullptr == name) {
        return nullptr;
    }

    for (auto &uniform : m_uniforms) {
        if (uniform->m_name == name) {
            return uniform;
        }
    }

    return nullptr;
}

RenderBatchData *PassData::getBatchById(const c8 *id) const {
    if (nullptr == id) {
        return nullptr;
    }

    for (ui32 i = 0; i < mMeshBatches.size(); ++i) {
        if (0 == strncmp(mMeshBatches[i]->m_id, id, strlen(id))) {
            return mMeshBatches[i];
        }
    }

    return nullptr;
}

static constexpr size_t MaxSubmitCmds = 500;

Frame::Frame() :
        m_newPasses(),
        m_submitCmds(),
        m_submitCmdAllocator(),
        m_uniforBuffers(nullptr),
        m_pipeline(nullptr) {
    m_submitCmdAllocator.reserve(MaxSubmitCmds);
}

Frame::~Frame() {
    delete[] m_uniforBuffers;
    m_uniforBuffers = nullptr;
}

void Frame::init(TArray<PassData*> &newPasses) {
    if (newPasses.isEmpty()) {
        return;
    }

    for (auto newPasse : newPasses) {
        m_newPasses.add(newPasse);
    }
    m_uniforBuffers = new UniformBuffer[newPasses.size()];
}

FrameSubmitCmd *Frame::enqueue(const char *passId, const char *batchId) {
    FrameSubmitCmd *cmd = m_submitCmdAllocator.alloc();
    if (nullptr != cmd) {
        cmd->m_passId = passId;
        cmd->m_batchId = batchId;
        m_submitCmds.add(cmd);
    }

    return cmd;
}

UniformDataBlob::UniformDataBlob() :
        m_data(nullptr),
        m_size(0) {
    // empty
}

UniformDataBlob::~UniformDataBlob() {
    clear();
}

void *UniformDataBlob::getData() const {
    return m_data;
}

void UniformDataBlob::clear() {
    ::free(m_data);
    m_data = nullptr;
    m_size = 0;
}

UniformDataBlob *UniformDataBlob::create(ParameterType type, size_t arraySize) {
    UniformDataBlob *blob = new UniformDataBlob;
    switch (type) {
        case ParameterType::PT_Int:
            blob->m_size = sizeof(i32);
            break;
        case ParameterType::PT_Float:
            blob->m_size = sizeof(f32);
            break;
        case ParameterType::PT_Float2:
            blob->m_size = sizeof(f32) * 2;
            break;
        case ParameterType::PT_Float3:
            blob->m_size = sizeof(f32) * 3;
            break;
        case ParameterType::PT_Mat4:
            blob->m_size = sizeof(f32) * 16;
            break;
        case ParameterType::PT_Mat4Array:
            blob->m_size = sizeof(f32) * 16 * arraySize;
            break;
        default:
            blob->m_size = 0;
            break;
    }
    blob->m_data = ::malloc(blob->m_size);
    ::memset(blob->m_data, 0, blob->m_size);

    return blob;
}

UniformVar::UniformVar() :
        m_name(""),
        m_type(ParameterType::PT_None),
        m_numItems(1),
        m_next(nullptr) {
    // empty
}

ui32 UniformVar::getParamDataSize(ParameterType type, ui32 arraySize) {
    ui32 size(0);
    switch (type) {
        case ParameterType::PT_Int:
            size = sizeof(i32);
            break;
        case ParameterType::PT_Float:
            size = sizeof(f32);
            break;
        case ParameterType::PT_Float2:
            size = sizeof(f32) * 2;
            break;
        case ParameterType::PT_Float3:
            size = sizeof(f32) * 3;
            break;
        case ParameterType::PT_Mat4:
            size = sizeof(f32) * 16;
            break;
        case ParameterType::PT_Mat4Array:
            size = sizeof(f32) * 16 * arraySize;
            break;

        default:
            break;
    }

    return size;
}

UniformVar *UniformVar::create(const String &name, ParameterType type,
        ui32 arraySize) {
    if (name.empty()) {
        osre_debug(Tag, "Empty name for parameter.");
        return nullptr;
    }

    UniformVar *param = new UniformVar;
    param->m_name = name;
    param->m_type = type;
    param->m_numItems = arraySize;
    param->m_data.m_size = UniformVar::getParamDataSize(type, arraySize);
    param->m_data.m_data = new uc8[param->m_data.m_size];
    param->m_next = nullptr;
    ::memset(param->m_data.m_data, 0, param->m_data.m_size);

    return param;
}

void UniformVar::destroy(UniformVar *param) {
    if (nullptr != param) {
        delete param;
    }
}

size_t UniformVar::getSize() {
    // len of name | name | buffer
    return m_name.size() + 1 + m_data.m_size;
}

static const c8 *GlslVersionStringArray[(size_t)GLSLVersion::Count] = {
    "1.10",
    "1.20",
    "1.30",
    "1.40",
    "1.50",
    "3.30",
    "4.00",
    "4.10",
    "4.20",
    "4.30",
    "4.40",
    "4.50",
    "4.60"
};

GLSLVersion getGlslVersionFromeString(const c8 *versionString) {
    GLSLVersion version = GLSLVersion::Invalid;
    if (versionString == nullptr) {
        return version;
    }

    std::string str(versionString);
    for (size_t i = 0; i < (size_t)GLSLVersion::Count; ++i) {
        const String v = GlslVersionStringArray[i];
        std::string::size_type pos = str.find(v);
        if (pos != std::string::npos) {
            version = static_cast<GLSLVersion>(i);
            break;
        }
    }

    return version;
}

} // Namespace RenderBackend
} // Namespace OSRE
