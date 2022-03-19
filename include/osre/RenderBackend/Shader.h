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

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

///	@brief  This class represents a container for all used shaders in the OSRE-runtime.
class OSRE_EXPORT Shader {
public:
    /// @brief The default class constructor.
    Shader();

    ///	@brief  The class destructor.
    ~Shader();

    void addVertexAttribute(const String &name);
    void addVertexAttributes(const String *names, size_t numAttribues);

    size_t getNumVertexAttributes() const;

    const c8 *getVertexAttributeAt(size_t index) const;

    void addUniformBuffer(const String &name);

    size_t getNumUniformBuffer() const;

    const c8 *getUniformBufferAt(size_t index) const;

    /// @brief  Will set the sours for a given shader type.
    /// @param  type    The shader type.
    /// @param  src     The source for the shader type.
    void setSource(ShaderType type, const String &src);

    bool hasSource(ShaderType type) const;

    const c8 *getSource(ShaderType type) const;

    /// @brief  Will return the type of a shader from its extension.
    /// @param  extension   The extension.
    /// @return The shader type.
    static ShaderType getTypeFromeExtension(const String &extension);

    OSRE_NON_COPYABLE(Shader)

private:
    enum CompileState {
        Updated = 0,
        Compiled,
        Error,
        MaxCompileState
    };

    CPPCore::TArray<String> mUniformBuffer;
    CPPCore::TArray<String> mVertexAttributes;
    String m_src[MaxShaderTypes];
    CompileState m_compileState[MaxCompileState];
};

inline void Shader::addVertexAttribute( const String &name ) {
    mVertexAttributes.add(name);
}

inline void Shader::addVertexAttributes( const String *names, size_t numAttributes ) {
    if (numAttributes == 0 || nullptr == names) {
        return;
    }

    for (size_t i = 0; i < numAttributes; ++i) {
        mVertexAttributes.add(names[i]);
    }
}

inline size_t Shader::getNumVertexAttributes() const {
    return mVertexAttributes.size();
}

inline const c8 *Shader::getVertexAttributeAt( size_t index ) const {
    if (index >= mVertexAttributes.size()) {
        return nullptr;
    }

    return mVertexAttributes[index].c_str();
}

inline void Shader::addUniformBuffer(const String &name) {
    mUniformBuffer.add(name);
}

inline size_t Shader::getNumUniformBuffer() const {
    return mUniformBuffer.size();
}

inline const c8 *Shader::getUniformBufferAt(size_t index) const {
    if (index >= mUniformBuffer.size()) {
        return nullptr;
    }

    return mUniformBuffer[index].c_str();
}

inline bool Shader::hasSource(ShaderType type) const {
    return !m_src[static_cast<size_t>(type)].empty();
}


///	@brief  This class is sued to load a shader.
class OSRE_EXPORT ShaderLoader {
public:
    /// @brief  The default class constructor.
    ShaderLoader();
    
    /// @brief  The class destructor.
    virtual ~ShaderLoader();
    
    /// @brief 
    /// @param uri 
    /// @param shader 
    /// @return 
    virtual size_t load(const IO::Uri &uri, Shader *shader);
    
    /// @brief 
    /// @param shader 
    /// @return 
    virtual bool unload(Shader *shader);
};

/// @brief 
class OSRE_EXPORT ShaderResource : public Common::TResource<Shader, ShaderLoader> {
public:
    /// @brief 
    /// @param shaderName 
    /// @param uri 
    ShaderResource(const String &shaderName, const IO::Uri &uri);
    
    ///	@brief
    ~ShaderResource() override;

protected:
    /// @brief 
    /// @param uri 
    /// @param loader 
    /// @return 
    Common::ResourceState onLoad(const IO::Uri &uri, ShaderLoader &loader) override;
    
    /// @brief 
    /// @param loader 
    /// @return 
    Common::ResourceState onUnload(ShaderLoader &loader) override;
};

} // Namespace RenderBackend
} // Namespace OSRE
