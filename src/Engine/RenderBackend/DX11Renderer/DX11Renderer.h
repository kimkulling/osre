#pragma once

#include <DirectXMath.h>

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

using namespace DirectX;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct D3D11_INPUT_ELEMENT_DESC;

namespace OSRE {

namespace Platform {
    class AbstractSurface;
}

namespace RenderBackend {

struct RenderCmd {
    ID3D11Buffer *m_vb;
    ID3D11Buffer *m_ib;
};

class DX11Renderer {
public:
    DX11Renderer();
    ~DX11Renderer();
    bool create(Platform::AbstractSurface *surface);
    bool destroy();
    ID3D11Buffer *createBuffer(BufferType type, BufferData *bd);
    void releaseBuffer(ID3D11Buffer *buffer);
    D3D11_INPUT_ELEMENT_DESC *createVertexLayout(VertexLayout *layout, Shader *shader);
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
    XMMATRIX m_projectionMatrix;
    XMMATRIX m_worldMatrix;
    XMMATRIX m_orthoMatrix;
};

} // Namespace RenderBackend
} // Namespace OSRE
