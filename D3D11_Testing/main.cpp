#include "testing.h"
#include "Engine.h"


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hInstancePrev,
	LPWSTR pCmdLine, int nShowCmd )
{
	Engine* engine = new Engine(hInstance, L"Direct3D 11 Testing", nShowCmd);
	engine->Run();

	return 0;
}

