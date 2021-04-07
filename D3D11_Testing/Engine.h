#pragma once
#include "testing.h"


class Engine
{
public:
	Engine(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
	~Engine();

	void Run();

private:
	static LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT uMsg,
		_In_ WPARAM wParam, _In_ LPARAM lParam);
	void InitDirectX(HINSTANCE hInstance, BOOL vSync);
	void Update(float deltaTime);

	LONG g_WindowWidth = 1280;
	LONG g_WindowHeight = 720;
	HWND g_WindowHandle = 0;

	const BOOL g_EnableVSync = false;

	// Direct3D device and swap chain.
	Microsoft::WRL::ComPtr<ID3D11Device> g_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> g_d3dSwapChain;

	// Render target view for the back buffer of the swap chain.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_d3dRenderTargetView;
	// Depth/stencil view for use as a depth buffer.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> g_d3dDepthStencilView;
	// A texture to associate to the depth stencil view.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> g_d3dDepthStencilBuffer;
	// Define the functionality of the depth/stencil stages.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> g_d3dDepthStencilState;

	// Define the functionality of the rasterizer stage.
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> g_d3dRasterizerState;
	D3D11_VIEWPORT g_Viewport = { 0 };

	// Shader resources
	enum ConstantBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> g_d3dConstantBuffers[NumConstantBuffers];

	DirectX::XMMATRIX g_WorldMatrix;
	DirectX::XMMATRIX g_ViewMatrix;
	DirectX::XMMATRIX g_ProjectionMatrix;

	// Vertex data for a colored cube.
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
	};

	VertexPosColor g_Vertices[8] =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
		{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
		{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
		{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
		{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
		{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
		{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) }  // 7
	};

	WORD g_Indicies[36] =
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};
};

