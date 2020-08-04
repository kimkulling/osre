#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <DirectXMath.h>	
#pragma warning(push)
#   pragma warning(disable : 4201)
#   include <glm/glm.hpp>
#   include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

using namespace DirectX;

// Forward declarations	
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
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;

namespace OSRE {
namespace RenderBackend {

struct RenderCmd {
    ID3D11Buffer *m_vb;
    ID3D11Buffer *m_ib;
};

struct ConstantBuffer {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

struct DX11Shader {
    ID3D10Blob * m_vsBuffer;
    ID3D11VertexShader * m_vertexShader;
    ID3D11PixelShader * m_pixelShader;
};

struct DX11VertexLayout {
    D3D11_INPUT_ELEMENT_DESC * m_desc;
};

template<class T>
inline
    void SafeRelease(T *iface) {
    if (nullptr != iface) {
        iface->Release();
        iface = nullptr;
    }
}

}
}
