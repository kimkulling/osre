#pragma once

#include <osre/Common/osre_common.h>
#include <osre/UI/Widget.h>

namespace OSRE {
    
// forward declarations
namespace RenderBackend {
    struct Geometry;
}

namespace UI {

struct Style;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class UIRenderUtils {
public:
    static RenderBackend::Geometry *createRectFromStyle( WidgetType Type, const RectUI &rect, const Style &style );
};

} // Namespace UI
} // Namespace OSRE
