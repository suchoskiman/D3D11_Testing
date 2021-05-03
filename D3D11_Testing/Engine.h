#pragma once
#include "testing.h"
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"


class Engine
{
public:
	Engine(HINSTANCE hInstance, LPWSTR pCmdLine, int nCmdShow);
	~Engine();

	void Run();

private:
	static LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT uMsg,
		_In_ WPARAM wParam, _In_ LPARAM lParam);
	void InitEngine(HINSTANCE hInstance, BOOL vSync);
	void Update(float deltaTime);
	void Render();

	HINSTANCE hInstance;
	LPCWSTR name;
	int show;

	LONG g_WindowWidth = 1280;
	LONG g_WindowHeight = 720;
	HWND g_WindowHandle = 0;

	const BOOL g_EnableVSync = false;

	// Shader resources
	enum ConstantBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> g_d3dConstantBuffers[NumConstantBuffers];
};

