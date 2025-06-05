/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Profiling/FPSCounter.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/OGLRenderer/OGLCommon.h"
#include "Platform/AbstractTimer.h"

#include <cppcore/Container/TArray.h>

#include <map>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace IO {
	class Stream;
	class Uri;

} // namespace IO

namespace Platform {
	class AbstractOGLRenderContext;
} // namespace Platform

namespace RenderBackend {

class OGLShader;
class Shader;

struct ClearState;
struct CullState;
struct BlendState;
struct SamplerState;
struct ClearState;
struct StencilState;
struct OGLBuffer;
struct OGLVertexArray;
struct OGLVertexAttribute;
struct OGLTexture;
struct OGLFrameBuffer;
struct RenderCmdData3DView;
struct UniformVar;
struct PrimitiveGroup;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to wrap all OpenGL-specific calls ( version 4.x and higher ).
//-------------------------------------------------------------------------------------------------
class OGLRenderBackend {
public:
	using VertAttribArray = cppcore::TArray<OGLVertexAttribute *>;

	/// @brief The default class constructor.
	OGLRenderBackend();

	/// @brief The class destructor.
	~OGLRenderBackend();

	/// @brief Will enumerate the GPU capabilities.
	void enumerateGPUCaps();

	/// @brief Will set the clear color.
	/// @param[in] clearColor  The new clear color.
	void setClearColor(const Color4 &clearColor);

	/// @brief Will set the requested global matrix for the frame.
	void setMatrix(MatrixType type, const glm::mat4 &mat);

	/// @brief All matrix values will be applied to the current frame.
	void applyMatrix();

	/// @brief 
	const glm::mat4 &getMatrix(MatrixType type) const;

	/// @brief 
	bool create(Platform::AbstractOGLRenderContext *renderCtx);

	/// @brief 
	bool destroy();
	void setTimer(Platform::AbstractTimer *timer);
	void setRenderContext(Platform::AbstractOGLRenderContext *renderCtx);
	void clearRenderTarget(const ClearState &clearState);
	void setViewport(const Viewport &viewport);
	OGLBuffer *createBuffer(BufferType type);
    OGLBuffer *getBufferById(guid bufferId);
	void bindBuffer(ui32 handle);
	void bindBuffer(OGLBuffer *pBuffer);
	void unbindBuffer(OGLBuffer *pBuffer);
	void copyDataToBuffer(OGLBuffer *pBuffer, void *pData, size_t size, BufferAccessType usage);
	void releaseBuffer(OGLBuffer *pBuffer);
	void releaseAllBuffers();
	bool createVertexCompArray(const VertexLayout *layout, OGLShader *pShader, VertAttribArray &attributes);
	bool createVertexCompArray(VertexType type, OGLShader *pShader, VertAttribArray &attributes);
	void releaseVertexCompArray(cppcore::TArray<OGLVertexAttribute *> &attributes);
	OGLVertexArray *createVertexArray();
	bool bindVertexLayout(OGLVertexArray *pVertexArray, OGLShader *pShader, size_t stride, GLint loc,
			OGLVertexAttribute *attrib);
	bool bindVertexLayout(OGLVertexArray *pVertexArray, OGLShader *pShader, size_t stride,
			const cppcore::TArray<OGLVertexAttribute *> &attributes);
	void destroyVertexArray(OGLVertexArray *pVertexArray);
	OGLVertexArray *getVertexArraybyId(ui32 id) const;
	void bindVertexArray(OGLVertexArray *pVertexArray);
	void unbindVertexArray();
	void releaseAllVertexArrays();
	OGLShader *createShader(const String &name, Shader *pShader);
	OGLShader *getShader(const String &name);
	bool useShader(OGLShader *pShader);
	OGLShader *getActiveShader() const;
	bool releaseShader(OGLShader *pShader);
	void releaseAllShaders();
	OGLTexture *createEmptyTexture(const String &name, TextureTargetType target, PixelFormatType pixelFormat, ui32 width, ui32 height, ui32 channels);
    OGLTexture *createDefaultTexture(TextureTargetType target, PixelFormatType pixelFormat, ui32 width, ui32 height);
	void updateTexture(OGLTexture *pOGLTextue, ui32 offsetX, ui32 offsetY, c8 *data, size_t size);
	OGLTexture *createTexture(const String &name, Texture *tex);
	OGLTexture *createTextureFromFile(const String &name, const IO::Uri &fileloc);
	OGLTexture *findTexture(const String &name) const;
	bool bindTexture(OGLTexture *pOGLTextue, TextureStageType stageType);
    bool unbindTexture( TextureStageType stageType);
	void releaseTexture(OGLTexture *pTexture);
	void releaseAllTextures();
	OGLParameter *createParameter(const String &name, ParameterType type, UniformDataBlob *blob, size_t numItems);
	OGLParameter *getParameter(const String &name) const;
	void setParameter(OGLParameter *param);
	void setParameter(OGLParameter **param, size_t numParam);
	void releaseAllParameters();
	size_t addPrimitiveGroup(PrimitiveGroup *grp);
	void releaseAllPrimitiveGroups();
    OGLFrameBuffer *createFrameBuffer(const String &name, ui32 width, ui32 height, PixelFormatType pixelFormat, bool depthBuffer);
	void bindFrameBuffer(OGLFrameBuffer *oglFB);
	OGLFrameBuffer *getFrameBufferByName(const String &name) const;
	void releaseFrameBuffer(OGLFrameBuffer *oglFB);
	void render(size_t grimpGrpIdx);
	void render(size_t primpGrpIdx, size_t numInstances);
	void renderFrame();
	void setFixedPipelineStates(const RenderStates &states);
    void setExtensions(const String &extensions);
    const String &getExtensions() const;
    
private:
    Color4 mClearColor;
    TransformMatrixBlock mMatrixBlock;
    Platform::AbstractOGLRenderContext *mRenderCtx;
	cppcore::TArray<OGLBuffer*> mBuffers;
	GLuint mActiveVB;
	GLuint mActiveIB;
	cppcore::TArray<OGLVertexArray*> mVertexArrays;
	GLuint mActiveVertexArray;
	cppcore::TArray<OGLShader*> mShaders;
	cppcore::TArray<OGLTexture*> mTextures;
    cppcore::TArray<OGLTexture*> mBindedTextures;
	cppcore::TArray<size_t> mFreeTexSlots;
	std::map<String, size_t> m_texLookupMap;
	cppcore::TArray<OGLParameter *> mParameters;
	OGLShader *mShaderInUse;
	cppcore::TArray<size_t> mFreeBufferSlots;
	cppcore::TArray<OGLPrimGroup*> mPrimitives;
	RenderStates *mFpState;
	Profiling::FPSCounter *mFpsCounter;
	OGLCapabilities mOglCapabilities;
	cppcore::TArray<OGLFrameBuffer*> mFrameFuffers;
    OGLDriverInfo mOGLDriverInfo;
};

} // Namespace RenderBackend
} // Namespace OSRE
