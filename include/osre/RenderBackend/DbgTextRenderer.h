#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {
    
struct BufferData;

class DbgTextRenderer {
public:
    DbgTextRenderer();
    ~DbgTextRenderer();
    void setupVertexData();
    void setCursor( ui32  x, ui32 y );
    void setScale( ui32 sx, ui32 sy );
    void addText( const String &text );
    void drawText();

private:
    BufferData *m_data;
    String m_text;
};

}
}
