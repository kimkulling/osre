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

#include "Common/osre_common.h"
#include "RenderBackend/RenderCommon.h"

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/TStaticArray.h>

namespace OSRE {
namespace RenderBackend {

///	@brief  This enum describes the different shader types, which are supported by the OSRE-engine.
enum class ShaderType {
    Invalid = -1,               ///< Enum for invalid enum.
    SH_VertexShaderType = 0,    ///< The shader is a vertex shader, used for each vertex.
    SH_GeometryShaderType,      ///< The shader is a geometry shader, used for tesselation.
    SH_TesselationShaderType,   ///< The tesselation evaluation shader.
    SH_FragmentShaderType,      ///< The shader is a fragment shader, used for rasterization.
    Count                       ///< Number of enums.
};

/// @brief The type to store shader
using ShaderSourceArray = cppcore::TStaticArray<String, static_cast<size_t>(ShaderType::Count)>;

//-------------------------------------------------------------------------------------------------
///	@brief  This class represents a container for all used shaders in the OSRE-runtime.
///
/// This class defines shader which will build a representation with the used API in the render thread.
/// You can think of this like a facade to access data in the shader itself.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Shader {
public:
    /// @brief The default class constructor.
    /// @param[in] name     The shader name.
    Shader(const String &name);

    ///	@brief  The class destructor.
    ~Shader() = default;

    /// @brief  Will a a new vertex attribute.
    /// @param  name    The name of the vertex attribute.
    void addVertexAttribute(const String &name);

    /// @brief  Will add an array of vertex attributes to the shader.
    /// @param  names           The names of the vertex attributes.
    /// @param  numAttribues    The number of attributes.
    void addVertexAttributes(const String *names, size_t numAttribues);

    /// @brief Will return the number of assigned vertex attributes.
    /// @return The number of assigned vertex attributes.
    size_t getNumVertexAttributes() const;

    /// @brief Will return the vertex attribute name at the given index.
    /// @param location     The index to look for.
    /// @return The name of the vertex attribute.
    const c8 *getVertexAttributeAt(size_t location) const;

    /// @brief  Will return the vertex location of the vertex attribute.
    /// @param  vertexAttribute  The vertex attribute to look for.
    /// @return The location used in the shader.
    size_t getLocation(const c8 *vertexAttribute) const;

    /// @brief  Will add an uniform buffer.
    /// @param  name    The buffer name.
    void addUniformBuffer(const String &name);

    /// @brief  Will return the number of attached uniform buffers.
    /// @return The number of buffers.
    size_t getNumUniformBuffer() const;

    /// @brief  Will return the uniform buffer at the given index.
    /// @param  index       The index.
    /// @return The buffer name or nullptr in case of an invalid index.
    const c8 *getUniformBufferAt(size_t index) const;

    /// @brief  Will set the sours for a given shader type.
    /// @param  type    The shader type.
    /// @param  src     The source for the shader type.
    void setSource(ShaderType type, const String &src);

    /// @brief  Will return true, if the shader type was assigned to source.
    /// @param  type     The shader type.
    /// @return true, if source was assigned, else false.
    bool hasSource(ShaderType type) const;

    /// @brief  Will return the assigned shader source for the given shader type
    /// @param  type     The shader type.
    /// @return The assigned source or an empty string.
    const c8 *getSource(ShaderType type) const;

    /// @brief  Will return the type of a shader from its extension.
    /// @param  extension   The extension.
    /// @return The shader type.
    static ShaderType getTypeFromeExtension(const String &extension);

    // No copying
    OSRE_NON_COPYABLE(Shader)

private:
    enum CompileState {
        Invalid = -1,
        Updated = 0,
        Compiled,
        Error,
        Count
    };
    String mName;
    StringArray mUniformBuffer;
    StringArray mVertexAttributes;
    String mSrc[static_cast<size_t>(ShaderType::Count)];
    CompileState mCompileState[Count];
};

inline size_t Shader::getNumVertexAttributes() const {
    return mVertexAttributes.size();
}

inline size_t Shader::getNumUniformBuffer() const {
    return mUniformBuffer.size();
}

inline bool Shader::hasSource(ShaderType type) const {
    return !mSrc[static_cast<size_t>(type)].empty();
}

///	@brief  This class is used to load a shader.
class OSRE_EXPORT ShaderLoader {
public:
    /// @brief  The default class constructor.
    ShaderLoader() = default;
    
    /// @brief  The default class destructor.
    virtual ~ShaderLoader() = default;
    
    /// @brief  Will load a shader from a given uri.
    /// @param  uri      The uri to load from.
    /// @param  shader   The shader instance to load in.
    /// @return The size in bytes.
    virtual size_t load(const IO::Uri &uri, Shader *shader);
    
    /// @brief  Will unload a loaded shader-
    /// @param  shader   The shader to unload.
    /// @return true, if unloaded.
    virtual bool unload(Shader *shader);
};

/// @brief  This class is used to represent the shader resource.
class OSRE_EXPORT ShaderResource : public Common::TResource<Shader, ShaderLoader> {
public:
    /// @brief  The class constructor with the name and the uri to load from.
    /// @param  shaderName  The name for the shader.
    /// @param  uri         The uri to load from.
    ShaderResource(const String &shaderName, const IO::Uri &uri);
    
    ///	@brief  The default class destructor.
    ~ShaderResource() override = default;

protected:
    /// @brief  The load callback handler.
    /// @param  uri     The uri to load from.
    /// @param  loader  The shader loader instance.
    /// @return The resource state.
    Common::ResourceState onLoad(const IO::Uri &uri, ShaderLoader &loader) override;
    
    /// @brief  The unload callback handler.
    /// @param  loader   The shader loader instance.
    /// @return The resource state.
    Common::ResourceState onUnload(ShaderLoader &loader) override;
};

} // Namespace RenderBackend
} // Namespace OSRE
