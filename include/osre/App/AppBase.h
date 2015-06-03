#pragma once

#include <osre/Common/Types.h>

namespace OSRE {

namespace Properties {
    class ConfigurationMap;
}

namespace App {
        
//-------------------------------------------------------------------------------------------------
///	@class		
///	@ingroup	
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AppBase {
public:
    AppBase( i32 argc, c8 *argv[], const String &supportedArgs = "api", const String &desc = "The render API" );
    virtual ~AppBase();
    virtual bool create( Properties::ConfigurationMap *config = nullptr );
    virtual bool destroy();
    virtual void update();
    virtual void requestNextFrame();
    virtual bool handleEvents();

protected:
    virtual bool onCreate( Properties::ConfigurationMap *config );
    virtual bool onDestroy();
        
private:
    struct Impl;
    Impl *m_impl;
};

}
}
