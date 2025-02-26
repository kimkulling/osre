#pragma once

#include "Common/osre_common.h"
#include "IO/Uri.h"
#include "RenderBackend/Shader.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
class TextureResource;

struct Texture;
struct UniformVar;

///	@brief  This enum describes the kind of build-in material.
enum class MaterialType {
    Invalid = -1,           ///< Enum for invalid enum.
    ShaderMaterial = 0,     ///< Material using a build-in shader assigned to its type of vertex.
    Count                   ///< Number of enums.
};

///	@brief  This enum describes the color type for the material.
enum class MaterialColorType : i32 {
    Invalid = -1,       ///< Init value
    Mat_Diffuse = 0,    ///< Diffuse color
    Mat_Specular,       ///< Specular color
    Mat_Ambient,        ///< Ambient color
    Mat_Emission,       ///< Emission color
    Count               ///< Number of enums.
};

enum class MaterialParameterType {
    Invalid = -1,
    Shineness,
    ShinenessStrength,
    Count
};

using TextureResourceArray = cppcore::TArray<RenderBackend::TextureResource*>;

static constexpr ui32 MaxMatColorType = static_cast<ui32>(MaterialColorType::Count);

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the materila description.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Material {
public:
    Material(const String &name, const IO::Uri &uri);
    ~Material();
    void setMaterialType(MaterialType matType);
    MaterialType getMaterialType() const;
    void createShader(const String &name, ShaderSourceArray &shaders);
    void setShader(Shader *shader);
    Shader *getShader() const;
    bool hasShader() const;
    void setColor(MaterialColorType colorType, const Color4 &color);
    const Color4 &getColor(MaterialColorType colorType) const;
    void setFloatParameter(MaterialParameterType, f32 value);
    f32 getFloatParameter(MaterialParameterType) const;

    OSRE_NON_COPYABLE(Material)

    FastString mName;
    MaterialType mType;
    size_t mNumTextures;
    Texture **mTextures;
    Shader *mShader;
    ui32 mNumParameters;
    UniformVar *mParameters;
    Color4 mColor[MaxMatColorType];
    f32 mShineness;
    f32 mShinenessStrength;
    IO::Uri mUri;
};

inline void Material::setShader(Shader *shader) {
    mShader = shader;
}

inline Shader *Material::getShader() const {
    return mShader;
}

inline bool Material::hasShader() const {
    return nullptr != mShader;
}

inline void Material::setColor(MaterialColorType colorType, const Color4 &color) {
    switch (colorType) {
        case MaterialColorType::Mat_Diffuse:
            mColor[(size_t) MaterialColorType::Mat_Diffuse] = color;
            break;
        case MaterialColorType::Mat_Specular:
            mColor[(size_t) MaterialColorType::Mat_Specular] = color;
            break;
        case MaterialColorType::Mat_Ambient:
            mColor[(size_t) MaterialColorType::Mat_Ambient] = color;
            break;
        case MaterialColorType::Mat_Emission:
            mColor[(size_t) MaterialColorType::Mat_Emission] = color;
            break;
        case MaterialColorType::Invalid:
        case MaterialColorType::Count:
            break;
    }
}

inline const Color4 &Material::getColor(MaterialColorType colorType) const {
    if (colorType == MaterialColorType::Invalid || colorType == MaterialColorType::Count) {
        static Color4 dummy(1,1,1,1);
        return dummy;
    }
    return mColor[(size_t)colorType];
}

inline void Material::setFloatParameter(MaterialParameterType type, f32 value) {
    switch (type) {
        case MaterialParameterType::Shineness:
            mShineness = value;
            break;
        case MaterialParameterType::ShinenessStrength:
            mShinenessStrength = value;
            break;
        case MaterialParameterType::Invalid:
        case MaterialParameterType::Count:
        default:
            break;
    }
}

inline f32 Material::getFloatParameter(MaterialParameterType type) const {
    switch (type) {
        case MaterialParameterType::Shineness:
            return mShineness;
        case MaterialParameterType::ShinenessStrength:
            return mShinenessStrength;
        case MaterialParameterType::Invalid:
        case MaterialParameterType::Count:
        default:
            break;
    }
    return 0.0f; 

}

} // namespace RenderBackend
} // namespace OSRE
