#pragma once

#include <osre/Common/osre_common.h>
#include <osre/IO/Uri.h>
#include <cppcore/Container/TArray.h>
#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
class TextureResource;

struct Texture;
struct UniformVar;


///	@brief  This enum describes the kind of build-in material.
enum class MaterialType {
    ShaderMaterial = 0, ///< Material using a build-in shader assigned to its type of vertex.
    NumMaterialTypes, ///< Number of enums.

    InvalidMaterialType ///< Enum for invalid enum.
};

///	@brief  This enum describes the color type for the material.
enum class MaterialColorType : ui32 {
    Mat_Diffuse = 0, ///<
    Mat_Specular, ///<
    Mat_Ambient, ///<
    Mat_Emission, ///<
    NumMaterialColorTypes, ///< Number of enums.

    InvalidMaterialColorType ///< Enum for invalid enum.
};

using TextureResourceArray = cppcore::TArray<RenderBackend::TextureResource *>;

static constexpr ui32 MaxMatColorType = static_cast<ui32>(MaterialColorType::NumMaterialColorTypes);


//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Material {
public:
    String m_name;
    MaterialType m_type;
    size_t m_numTextures;
    Texture **m_textures;
    Shader *m_shader;
    ui32 m_numParameters;
    UniformVar *m_parameters;
    Color4 m_color[MaxMatColorType];
    f32 mShineness;
    f32 mShinenessStrength;
    IO::Uri mUri;

    Material(const String &name, const IO::Uri &uri);
    ~Material();
    void setMaterialType(MaterialType matType);
    MaterialType getMaterialType() const;
    void createShader(ShaderSourceArray &shaders);
    Shader *getShader() const;

    OSRE_NON_COPYABLE(Material)
};

inline Shader *Material::getShader() const {
    return m_shader;
}

} // namespace RenderBackend
} // namespace OSRE
