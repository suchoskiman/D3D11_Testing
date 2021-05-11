#include "Renderer.h"

Renderer::Renderer() :
	m_Device(NULL), m_DeviceContext(NULL), m_SwapChain(NULL), m_RenderTargetView(NULL),
	m_DepthStencilView(NULL), m_DepthStencilState(NULL),m_DepthStencilBuffer(NULL),
	m_VertexShader(NULL), m_PixelShader(NULL), m_InputLayout(NULL), m_Rasterizer(NULL)
{
	for (int buffer = 0; buffer < CBNumBuffers; buffer++)
	{
		m_ConstantBuffer[buffer] = NULL;
	}
}

Renderer::~Renderer()
{
	for (int buffer = 0; buffer < CBNumBuffers; buffer++)
		m_ConstantBuffer[buffer]->Release();
}

int Renderer::InitRenderer(HWND handle)
{
	// create the device and swap chain
	ComPtr<IDXGIFactory> factory;
	ComPtr<IDXGIAdapter> adapter;
	HRESULT hr = CreateDXGIFactory(__uuidof(&factory), (void**)factory.GetAddressOf());
	if (FAILED(hr))
	{
		return -1;
	}
	hr = factory->EnumAdapters(0, adapter.GetAddressOf());
	if (FAILED(hr))
	{
		return -1;
	}

	RECT clientRect;
	GetClientRect(handle, &clientRect);

	// Compute the exact client dimensions. This will be used
	// to initialize the render targets for our swap chain.
	unsigned int clientWidth = clientRect.right - clientRect.left;
	unsigned int clientHeight = clientRect.bottom - clientRect.top;

	DXGI_SAMPLE_DESC multisampling = { 8, 0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = handle;
	swapChainDesc.SampleDesc = multisampling;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// These are the feature levels that we will accept.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,

	};

	// This will be the feature level that 
	// is used to create our device and swap chain.
	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &featureLevel,
		&m_DeviceContext);

	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &featureLevel,
			&m_DeviceContext);
	}

	if (FAILED(hr))
	{
		return -1;
	}

	// The Direct3D device and swap chain were successfully created.
	// Now we need to initialize the buffers of the swap chain.
	// Next initialize the back buffer of the swap chain and associate it to a 
	// render target view.
	ID3D11Texture2D* backBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return -1;
	}

	hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
	if (FAILED(hr))
	{
		return -1;
	}

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = clientWidth;
	depthStencilBufferDesc.Height = clientHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc = multisampling;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_Device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_DepthStencilBuffer);
	if (FAILED(hr))
	{
		return -1;
	}

	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, &m_DepthStencilView);
	if (FAILED(hr))
	{
		return -1;
	}

	// Setup depth/stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = m_Device->CreateDepthStencilState(&depthStencilStateDesc, &m_DepthStencilState);

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_Rasterizer);
	if (FAILED(hr))
	{
		return -1;
	}

	// Initialize the viewport to occupy the entire client area.
	m_Viewport.Width = static_cast<float>(clientWidth);
	m_Viewport.Height = static_cast<float>(clientHeight);
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	// set up shaders
	ComPtr<ID3D10Blob> vertexShaderBlob;
	ComPtr<ID3D10Blob> pixelShaderBlob;

#ifndef _DEBUG
	hr = D3DReadFileToBlob(L"VertexShader.cso", vertexShaderBlob.GetAddressOf());
#else
	hr = D3DReadFileToBlob(L"VertexShader_d.cso", vertexShaderBlob.GetAddressOf());
#endif
	if (FAILED(hr))
	{
		return -1;
	}
	m_Device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
	m_Device->CreateInputLayout(g_VertexLayout, 2, vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), m_InputLayout.GetAddressOf());

#ifndef _DEBUG
	hr = D3DReadFileToBlob(L"PixelShader.cso", pixelShaderBlob.GetAddressOf());
#else
	hr = D3DReadFileToBlob(L"PixelShader_d.cso", pixelShaderBlob.GetAddressOf());
#endif
	if (FAILED(hr))
	{
		return -1;
	}
	m_Device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());

	// create constant buffer
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	for (int buffer = 0; buffer < CBNumBuffers; buffer++)
	{
		hr = m_Device->CreateBuffer(&constantBufferDesc, NULL, &m_ConstantBuffer[buffer]);
		if (FAILED(hr)) return -1;
	}

	return 0;
}

void Renderer::Draw(Mesh& mesh, Camera& camera)
{
	const UINT vertexStride = sizeof(Vertex);
	const UINT offset = 0;
	FLOAT color[4] = { 0.9f, 0.9f, 0.9f, 1.0f };

	// update constant buffer
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT4X4 viewMatrix = camera.GetViewMatrix();
	XMFLOAT4X4 modelMatrix = mesh.GetModelMatrix();
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f),
		1280 / (float)720, 0.001f, 1000.0f));
	m_DeviceContext->UpdateSubresource(m_ConstantBuffer[CBProjection], 0, NULL,
		&projectionMatrix, 0, 0);
	m_DeviceContext->UpdateSubresource(m_ConstantBuffer[CBView], 0, NULL,
		&viewMatrix, 0, 0);
	m_DeviceContext->UpdateSubresource(m_ConstantBuffer[CBModel], 0, NULL,
		&modelMatrix, 0, 0);

	// reset screen to a base color
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(),
		D3D10_CLEAR_DEPTH || D3D10_CLEAR_STENCIL, 1.0f, 0);

	// prepare input assembler
	m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetVertexBuffers(0, 1, mesh.GetD3DVertexBuffer(m_Device).GetAddressOf(),
		&vertexStride, &offset);
	m_DeviceContext->IASetIndexBuffer(mesh.GetD3DIndexBuffer(m_Device).Get(), DXGI_FORMAT_R16_UINT,
		offset);
	m_DeviceContext->IASetInputLayout(m_InputLayout.Get());

	m_DeviceContext->VSSetShader(m_VertexShader.Get(), NULL, 0);
	m_DeviceContext->VSSetConstantBuffers(0, CBNumBuffers, m_ConstantBuffer);

	m_DeviceContext->RSSetState(m_Rasterizer.Get());
	m_DeviceContext->RSSetViewports(1, &m_Viewport);

	m_DeviceContext->PSSetShader(m_PixelShader.Get(), NULL, 0);

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(),
		m_DepthStencilView.Get());
	m_DeviceContext->OMGetDepthStencilState(m_DepthStencilState.GetAddressOf(), 0);

	m_DeviceContext->DrawIndexed(mesh.GetIndexBufferCount(), 0, 0);
}

void Renderer::Present()
{
	m_SwapChain->Present(0, 0);
}