#pragma once

#include <osre/Common/Object.h>

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@class		
///	@ingroup	
///
///	@brief
//-------------------------------------------------------------------------------------------------
class FontBase : Common::Object {
public:
    FontBase( const String &name );
    virtual ~FontBase();
    virtual void setSize( ui32 size );
    virtual ui32 getSize() const;

private:
    ui32 m_size;
};

}
}