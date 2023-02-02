#include "Engine/Engine.h"

//int APIENTRY WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmd, _In_ int nCmd) c28251
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR lpCmd, INT nCmd)
{
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		Debug::Log(hr, L"CoInitialize ½ÇÆÐ!");
		return -1;
	}
	Engine engine;
	if(engine.Initialize(hInst, L"DirectX11", L"StudyDX11", 800, 600))
	{
		engine.Start();
		while (engine.ProcessMessages())
		{
			engine.Update();
			engine.Render();
		}
	}

	CoUninitialize();
	return 0;
}