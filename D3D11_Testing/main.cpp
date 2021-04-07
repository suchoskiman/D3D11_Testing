#include "testing.h"
#include "Engine.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hInstancePrev,
	PWSTR pCmdLine, int nShowCmd )
{
	Engine* myApp = new Engine(hInstance, pCmdLine, nShowCmd);
	myApp->Run();

	return 0;
}

