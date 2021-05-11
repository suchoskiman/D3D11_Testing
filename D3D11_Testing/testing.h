#pragma once
#define WIN32_LEAN_AND_MEAN

// System Includes
#include <windows.h>
#include <wrl.h>
#include <combaseapi.h>
#include <timeapi.h>

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// STL includes
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Linking libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

using namespace DirectX;
using namespace Microsoft::WRL;