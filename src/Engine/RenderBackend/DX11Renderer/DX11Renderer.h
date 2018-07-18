/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "DX11Common.h"

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/THashMap.h>

using namespace DirectX;

namespace OSRE {

namespace Platform {
    class AbstractWindow;
}

namespace RenderBackend {

class DX11Renderer {
public:
    DX11Renderer();
    ~DX11Renderer();
    bool create(Platform::AbstractWindow *surface);
    bool destroy();
    ID3D11Buffer *createBuffer(BufferType type, BufferData *bd, BufferAccessType usage);
    void releaseBuffer(ID3D11Buffer *buffer);
    DX11VertexLayout *createVertexLayout(VertexLayout *layout, DX11Shader *shader);
    DX11Shader *createShader(Shader *shader);
    void setMatrix(MatrixType type, const glm::mat4 &mat);
    const glm::mat4 &getMatrix(MatrixType type) const;
    void setConstantBuffer(ui32 bufferNumber, ID3D11Buffer *constantBuffer);
    void beginScene(Color4 &clearColor);
    void render(RenderCmd *cmd);
    void endScene();

private:
    bool m_vsync_enabled;
    ui32 m_videoCardMemory;
    c8 m_videoCardDescription[128];
    IDXGISwapChain *m_swapChain;
    ID3D11Device *m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_worldMatrix;
    glm::mat4 m_orthoMatrix;
    ID3D11Buffer* m_matrixBuffer;
    MatrixBufferType m_matrixBufferData;
};

} // Namespace RenderBackend
} // Namespace OSRE
