#include <osre/RenderBackend/DbgTextRenderer.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <vector>

namespace OSRE {
namespace RenderBackend {

extern const unsigned char *glyph[];

static const ui32 NumChars = 128;

static const String VsSrc =
	"layout(location = 0) in vec3 position;	      // object space vertex position\n"
	"layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
	"layout(location = 2) in vec2 texcoord0;	        // texture coordinate\n"
	"out vec2 vUV;\n"
	"\n"
	"void main() {\n"
	"    vec3 vertexPosition_homoneneousspace = position;\n"
	"    gl_Position = vec4(position, 1 );\n"
	"    // UV of the vertex. No special space for this one.\n"
	"    UV = texcoord0;\n"
	"};\n";

static const String FsSrc =
	"in vec2 vUV;\n"
	"// Output data\n"
	"out vec4 vFragColor;\n"
	"uniform sampler2D tex0;\n"

	"void main() {\n"
	"    vFragColor = texture( tex0, vUV );\n"
	"};\n";

TextRenderer2D::TextRenderer2D()
: m_data( nullptr )
, m_text()
, m_dirty( false )
, m_xCursor( -1 )
, m_yCursor( -1 )
, m_sx( -1 )
, m_sy( -1 ) {
	// empty
}

TextRenderer2D::~TextRenderer2D() {
	clear();
}

void TextRenderer2D::setupVertexData() {
	std::vector<RenderVert> vertices;
	for (ui32 i = 0; i < m_text.size(); i++) {
		RenderVert vert[ 4 ];

		vert[ 0 ].position = glm::vec3(m_xCursor + i*m_sx, m_yCursor + m_sy, 0);
		vert[ 1 ].position = glm::vec3(m_xCursor + i*m_sx + m_sx, m_yCursor + m_sy, 0);
		vert[ 2 ].position = glm::vec3(m_xCursor + i*m_sx + m_sx, m_yCursor, 0);
		vert[ 3 ].position = glm::vec3(m_xCursor + i*m_sx, m_yCursor, 0);

		char character = m_text[ i ];
		float uv_x = ( character % 16 ) / 16.0f;
		float uv_y = ( character / 16 ) / 16.0f;
		vert[ 0 ].tex0 = glm::vec2(uv_x, 1.0f - uv_y);
		vert[ 1 ].tex0 = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
		vert[ 2 ].tex0 = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - ( uv_y + 1.0f / 16.0f ));
		vert[ 3 ].tex0 = glm::vec2(uv_x, 1.0f - ( uv_y + 1.0f / 16.0f ));

		for (ui32 j = 0; j < 4; j++) {
			vertices.push_back(vert[ j ]);
		}
	}
	
	const ui32 vertSize(vertices.size() * sizeof(RenderVert));
	if ( m_data->m_size < vertSize) {
		if (nullptr != m_data) {
			BufferData::free( m_data );
		}
		m_data = BufferData::alloc( VertexBuffer, vertSize, ReadWrite );
		::memcpy( m_data->m_pData, &vertices[ 0 ], vertSize );
	}

	Geometry *geo = new Geometry;
	geo->m_numPrimGroups = m_text.size()*2;
	geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
	geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
	geo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * geo->m_numPrimGroups;
	geo->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
	geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

	// setup material
	geo->m_material = new Material;
	geo->m_material->m_numTextures = 0;
	geo->m_material->m_type = ShaderMaterial;
	geo->m_material->m_pShader = new Shader;
	geo->m_material->m_pShader->m_src[ SH_VertexShaderType ]   = VsSrc;
	geo->m_material->m_pShader->m_src[ SH_FragmentShaderType ] = FsSrc;

	// setup shader attributes and variables
	if (nullptr != geo->m_material->m_pShader) {
		ui32 numAttribs(ColorVert::getNumAttributes());
		const String *attribs(ColorVert::getAttributes());
		geo->m_material->m_pShader->m_attributes.add(attribs, numAttribs);
		geo->m_material->m_pShader->m_parameters.add("MVP");
	}
}

void TextRenderer2D::setCursor( ui32  x, ui32 y ) {
	m_xCursor = x;
	m_yCursor = y;
}

void TextRenderer2D::setScale( i32 sx, i32 sy ) {
	m_sx = sx;
	m_sy = sy;
}

void TextRenderer2D::addText( const String &text ) {
    m_text += text;
	m_dirty = true;
}

void TextRenderer2D::clear() {
	m_text.clear();
	BufferData::free(m_data);
	m_data = nullptr;
}

void TextRenderer2D::drawText() {
    if( m_text.empty() ) {
        return;
    }

	if (m_dirty) {
		setupVertexData();
		m_dirty = false;
	}
}

} // Namespace RenderBackend
} // Namespace OSRE
