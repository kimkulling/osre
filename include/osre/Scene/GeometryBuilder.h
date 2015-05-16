#pragma once

#include <osre/Common/Types.h>

namespace OSRE {

namespace RenderBackend {
    struct Geometry;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Scene::GeometryBuilder
///	@ingroup	Application
///
///	@brief  This class is used to create the geometry for base shapes like a triangle.
//-------------------------------------------------------------------------------------------------
class GeometryBuilder {
public:
    /// @brief  The class constructor.
    GeometryBuilder();

    /// @brief  The class destructor.
    ~GeometryBuilder();

    /// @brief  Will create a triangle geometry.
    /// @return The created geometry.
    RenderBackend::Geometry *createTriangle();

    /// @brief  Will create a box geometry.
    /// @return The created geometry.
    RenderBackend::Geometry *createBox( f32 w, f32 h, f32 d);
};

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
