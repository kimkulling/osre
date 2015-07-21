#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {
        
class DbgTextRenderer {
public:
    DbgTextRenderer();
    ~DbgTextRenderer();
    void setupTexture();
    void setCursor( ui32  x, ui32 y );
    void setScale( ui32 sx, ui32 sy );
    void addText( const String &text );
    void drawText();

private:
    String m_text;
};

}
}
