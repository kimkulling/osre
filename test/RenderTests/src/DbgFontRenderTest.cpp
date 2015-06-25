#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

class DbgFontRenderTest : public AbstractRenderTest {
public:
    DbgFontRenderTest()
    : AbstractRenderTest( "rendertest/dbgfontrendertest" ) {
        // empty
    }

    ~DbgFontRenderTest() {

    }

    virtual bool onCreate( RenderBackendService *rbSrv ) {
        return true;
    }

    virtual bool onDestroy( RenderBackendService *pRenderBackendSrv ) {
        return true;
    }

    virtual bool onRender( d32 timediff, RenderBackendService *pRenderBackendSrv ) {
        return true;
    }

};

ATTACH_RENDERTEST( DbgFontRenderTest )

}
}