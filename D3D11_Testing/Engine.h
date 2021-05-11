#pragma once
#include "testing.h"
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"


class Engine
{
public:
	Engine(HINSTANCE hInstance, LPCWSTR pCmdLine, int nCmdShow);
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

	LONG m_WindowWidth = 1280;
	LONG m_WindowHeight = 720;
	HWND m_WindowHandle = 0;

	const BOOL m_EnableVSync = false;

	Renderer* m_Renderer;
	std::vector<Mesh> m_Meshes;

	Camera m_Camera{ {0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 0.0f} };
	XMFLOAT4X4 m_Projection;
};

