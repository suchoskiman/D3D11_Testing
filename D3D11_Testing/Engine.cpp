#include "Engine.h"

Engine::Engine(HINSTANCE hInstance, PWSTR name, int nCmdShow):
    hInstance(hInstance), name(name), show(nCmdShow)
{

}

Engine::~Engine()
{

}

void Engine::Run()
{
    InitEngine(hInstance, g_EnableVSync);
    MSG msg = { 0 };

    static DWORD previousTime = timeGetTime();
    static const float targetFramerate = 30.0f;
    static const float maxTimeStep = 1.0f / targetFramerate;

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        DWORD currentTime = timeGetTime();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // Cap the delta time to the max time step (useful if your 
        // debugging and you don't want the deltaTime value to explode.
        deltaTime = std::min<float>(deltaTime, maxTimeStep);

        Update(deltaTime);
        Render();
    }
}

LRESULT Engine::WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    PAINTSTRUCT paintStruct;
    HDC hDC;

    switch (uMsg)
    {
    case WM_PAINT:
    {
        hDC = BeginPaint(hwnd, &paintStruct);
        EndPaint(hwnd, &paintStruct);
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void Engine::InitEngine(HINSTANCE hInstance, BOOL vSync)
{
    // create and show window
    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = &WindowProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = NULL;
    wndClass.hIcon = NULL;
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = L"Testing";
    wndClass.lpszClassName = name;

    if (!RegisterClassEx(&wndClass))
    {
        return;
    }

    RECT windowRect = { 0, 0, g_WindowWidth, g_WindowHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    g_WindowHandle = CreateWindow(name, L"Testing",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!g_WindowHandle)
    {
        return;
    }

    ShowWindow(g_WindowHandle, show);
    UpdateWindow(g_WindowHandle);
}

void Engine::Update(float deltaTime)
{
    return;
}

void Engine::Render()
{
    return;
}