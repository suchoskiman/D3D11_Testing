#pragma once

#include "testing.h"
#include "Layout.h"
#include "Mesh.h"
#include "Camera.h"

// forward declaration
class Mesh;

class Renderer
{
public:
	Renderer();
	~Renderer();

	int InitRenderer(HWND handle);

	void Draw(Mesh& mesh, Camera& camera);
	void Present();
private:
	// devices
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	
	// backbuffer and rtv
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	// depth stencil
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;

	// shaders
	ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3D11PixelShader> m_PixelShader;
	
	// ancillary structures
	ComPtr<ID3D11InputLayout> m_InputLayout;
	ComPtr<ID3D11RasterizerState> m_Rasterizer;
	D3D11_VIEWPORT m_Viewport;

	// constant buffer stuff
	enum ConstantBuffer
	{
		CBProjection,
		CBView,
		CBModel,
		CBNumBuffers
	};
	ID3D11Buffer* m_ConstantBuffer[CBNumBuffers];
};

