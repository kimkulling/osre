#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {
    
struct BufferData;

class TextRenderer2D {
public:
    TextRenderer2D();
    ~TextRenderer2D();
    void setupVertexData();
    void setCursor( ui32  x, ui32 y );
    void setScale( i32 sx, i32 sy );
    void addText( const String &text );
	void clear();
    void drawText();

private:
    BufferData *m_data;
    String m_text;
	bool m_dirty;
	i32 m_xCursor;
	i32 m_yCursor;
	i32 m_sx;
	i32 m_sy;
};

} // Namespace RenderBackend
} // Namespace OSRE
