#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace App {
        
//-------------------------------------------------------------------------------------------------
///	@class		
///	@ingroup	
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AppBase {
public:
    AppBase( i32 argc, c8 *argv[], const String &supportedArgs = "api" );
    virtual ~AppBase();
    virtual bool create();
    virtual bool destroy();
    virtual void update();
    virtual void requestNextFrame();
    virtual bool handleEvents();

protected:
    virtual bool onCreate();
    virtual bool onDestroy();
        
private:
    struct Impl;
    Impl *m_impl;
};

}
}
