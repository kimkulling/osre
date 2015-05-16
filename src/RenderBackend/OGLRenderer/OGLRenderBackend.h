#pragma once

#include <cppcore/Container/TArray.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>

#include "OGLCommon.h"

#include <map>

namespace OSRE {

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLShader;

struct OGLBuffer;
struct OGLVertexArray;
struct OGLVertexAttribute;
struct OGLTexture;
struct RenderCommand;
struct RenderCmdData3DView;
struct Shader;
struct Parameter;
struct PrimitiveGroup;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLRenderBackend
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OGLRenderBackend {
public:
    OGLRenderBackend();
    ~OGLRenderBackend();
    void setRenderContext( Platform::AbstractRenderContext *pRenderCtx );
    void clearRenderTarget( ui32 targets );
    void setViewport( i32 x, i32 y, i32 w, i32 h );
    OGLBuffer *createBuffer( BufferType type );
    void bindBuffer( ui32 handle );
    void bindBuffer( OGLBuffer *pBuffer );
    void unbindBuffer( OGLBuffer *pBuffer );
    void bufferData( OGLBuffer *pBuffer, void *pData, ui32 size, BufferAccessType usage );
    void releaseBuffer( OGLBuffer *pBuffer );
    void releaseAllBuffers();
    bool createVertexAttribArray( VertexType type, OGLShader *pShader, CPPCore::TArray<OGLVertexAttribute*> &attributes );
    void releasevertexAttribArray( CPPCore::TArray<OGLVertexAttribute*> &attributes );
    OGLVertexArray *createVertexArray();
    bool bindVertexAttribute( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, GLint loc, OGLVertexAttribute* attrib );
    bool bindVertexAttributes( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, const CPPCore::TArray<OGLVertexAttribute*> &attributes );
    void destroyVertexArray( OGLVertexArray *pVertexArray );
    void bindVertexArray( OGLVertexArray *pVertexArray );
    void unbindVertexArray( OGLVertexArray *pVertexArray );
    void releaseAllVertexArrays();
    OGLShader *createShader( const String &name, Shader *pShader );
    OGLShader *getShader( const String &name );
    bool useShader( OGLShader *pShader );
    OGLShader *getActiveShader() const;
    bool releaseShader( OGLShader *pShader );
    void releaseAllShaders();
    OGLTexture *createEmptyTexture( const String &name, TextureTargetType target, ui32 width, ui32 height, ui32 channels );
    void updateTexture( OGLTexture *pOGLTextue, ui32 offsetX, ui32 offsetY, c8 *data, ui32 size );
    OGLTexture *createTextureFromFile( const String &name, const String &filename );
    OGLTexture *findTexture( const String &name ) const;
    bool bindTexture( OGLTexture *pOGLTextue, TextureStageType stageType );
    void releaseTexture( OGLTexture *pTexture );
    void releaseAllTextures();
    OGLParameter *createParameter( const String &name, ParameterType type, ParamDataBlob *blob, ui32 numItems );
    OGLParameter *getParameter( const String &name ) const;
    void setParameter( OGLParameter *param );
    void setParameter( OGLParameter **param, ui32 numParam );
    void releaseAllParameters();
    ui32 addPrimitiveGroup( PrimitiveGroup *grp );
    void releaseAllPrimitiveGroups();
    void render( ui32 grimpGrpIdx );
    void render( ui32 primpGrpIdx, ui32 numInstances );
    void renderFrame();
    static ui32 getVertexSize( VertexType vertextype );

private:
    Platform::AbstractRenderContext *m_pRenderCtx;
    CPPCore::TArray<OGLBuffer*>      m_buffers;
    CPPCore::TArray<OGLVertexArray*> m_vertexarrays;
    CPPCore::TArray<OGLShader*>      m_shaders;
    CPPCore::TArray<OGLTexture*>     m_textures;
    CPPCore::TArray<ui32>            m_freeTexSlots;
    std::map<String, ui32>        m_texLookupMap;
    CPPCore::TArray<OGLParameter*>   m_parameters;
    OGLShader                       *m_pShaderInUse;
    CPPCore::TArray<ui32>            m_freeBufferSlots;
    CPPCore::TArray<OGLPrimGroup*>   m_primitives;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
