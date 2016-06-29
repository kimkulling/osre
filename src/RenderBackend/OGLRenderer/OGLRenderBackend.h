/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <cppcore/Container/TArray.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Profiling/FPSCounter.h>

#include "OGLCommon.h"
#include <map>

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
    class Uri;
}

namespace Platform {
    class AbstractRenderContext;
    class AbstractTimer;
}

namespace RenderBackend {

class OGLShader;
class FontBase;
class ClearState;

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
///	@class		::OSRE::RenderBackend::OGLRenderBackend
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OGLRenderBackend {
public:
    typedef CPPCore::TArray<OGLVertexAttribute*> VertAttribArray;

public:
    OGLRenderBackend();
    ~OGLRenderBackend();
    void setTimer( Platform::AbstractTimer *timer );
    void setRenderContext( Platform::AbstractRenderContext *pRenderCtx );
    void clearRenderTarget( const ClearState &clearState );
    void setViewport( i32 x, i32 y, i32 w, i32 h );
    OGLBuffer *createBuffer( BufferType type );
    void bindBuffer( ui32 handle );
    void bindBuffer( OGLBuffer *pBuffer );
    void unbindBuffer( OGLBuffer *pBuffer );
    void bufferData( OGLBuffer *pBuffer, void *pData, ui32 size, BufferAccessType usage );
    void releaseBuffer( OGLBuffer *pBuffer );
    void releaseAllBuffers();
    bool createVertexCompArray( const VertexLayout *layout, OGLShader *pShader, VertAttribArray &attributes );
    bool createVertexCompArray( VertexType type, OGLShader *pShader, VertAttribArray &attributes );
    void releaseVertexCompArray( CPPCore::TArray<OGLVertexAttribute*> &attributes );
    OGLVertexArray *createVertexArray();
    bool bindVertexLayout( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, GLint loc, OGLVertexAttribute* attrib );
    bool bindVertexLayout( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, const CPPCore::TArray<OGLVertexAttribute*> &attributes );
    void destroyVertexArray( OGLVertexArray *pVertexArray );
    OGLVertexArray *getVertexArraybyId( ui32 id ) const;
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
    OGLTexture *createTextureFromFile( const String &name, const IO::Uri &fileloc );
    OGLTexture *createTextureFromStream( const String &name, IO::Stream &stream, ui32 width, ui32 height, ui32 channels );
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
    FontBase *createFont( const IO::Uri &font );
	void selectFont( FontBase *font );
    FontBase *findFont( const String &name ) const;
    bool relaseFont( FontBase *font );
    void releaseAllFonts();
    static ui32 getVertexSize( VertexType vertextype );

private:
    Platform::AbstractRenderContext *m_renderCtx;
	CPPCore::TArray<OGLBuffer*>      m_buffers;
    CPPCore::TArray<OGLVertexArray*> m_vertexarrays;
	GLuint                           m_activeVertexArray;
    CPPCore::TArray<OGLShader*>      m_shaders;
    CPPCore::TArray<OGLTexture*>     m_textures;
    CPPCore::TArray<ui32>            m_freeTexSlots;
    CPPCore::TArray<FontBase*>       m_fonts;
    FontBase                        *m_activeFont;
    std::map<String, ui32>           m_texLookupMap;
    CPPCore::TArray<OGLParameter*>   m_parameters;
    OGLShader                       *m_shaderInUse;
    CPPCore::TArray<ui32>            m_freeBufferSlots;
    CPPCore::TArray<OGLPrimGroup*>   m_primitives;
    Profiling::FPSCounter           *m_fpsCounter;
};

} // Namespace RenderBackend
} // Namespace OSRE
