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

using TextureResourceArray = cppcore::TArray<RenderBackend::TextureResource*>;

static constexpr ui32 MaxMatColorType = static_cast<ui32>(MaterialColorType::Count);

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the materila description.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Material {
public:
    FastString mName;
    MaterialType mType;
    size_t mNumTextures;
    Texture **mTextures;
    Shader *mShader;
    ui32 mNumParameters;
    UniformVar *mParameters;
    Color4 m_color[MaxMatColorType];
    f32 mShineness;
    f32 mShinenessStrength;
    IO::Uri mUri;

    Material(const String &name, const IO::Uri &uri);
    ~Material();
    void setMaterialType(MaterialType matType);
    MaterialType getMaterialType() const;
    void createShader(const String &name, ShaderSourceArray &shaders);
    void setShader(Shader *shader);
    Shader *getShader() const;

    OSRE_NON_COPYABLE(Material)
};

inline void Material::setShader(Shader *shader) {
    mShader = shader;
}

inline Shader *Material::getShader() const {
    return mShader;
}

} // namespace RenderBackend
} // namespace OSRE
