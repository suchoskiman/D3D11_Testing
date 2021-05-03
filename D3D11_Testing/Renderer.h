#pragma once

#include "testing.h"
#include "Mesh.h"
#include "Camera.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class Renderer
{
public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 color;
	};

	Renderer();
	~Renderer();

	int InitRenderer();
	void Draw(Mesh<Vertex> mesh, Camera camera);
private:
	// devices
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	
	// backbuffer and rtv
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> M_RenderTargetView;

	// depth stencil
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;

	// shaders
	ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3D11PixelShader> m_PixelShader;

	// ancillary structures
	ComPtr<ID3D11RasterizerState> m_Rasterizer;
	D3D11_VIEWPORT m_Viewport;
};

