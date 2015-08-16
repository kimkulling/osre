#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {

namespace Scene {
    class Stage;
}

namespace Properties {
    class Setting;
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
    virtual bool create( Properties::Setting *config = nullptr );
    virtual bool destroy();
    virtual void update();
    virtual void requestNextFrame();
    virtual bool handleEvents();
    virtual Properties::Setting *getConfig() const;
    virtual Scene::Stage *createStage( const String &name );

protected:
    virtual bool onCreate( Properties::Setting *config );
    virtual bool onDestroy();
    virtual void onUpdate();

private:
    struct Impl;
    Impl *m_impl;
};

}
}
