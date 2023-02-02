#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInst, wstring title, wstring winClass, int w, int h)
{
	if (!window.Initialize(this, hInst, title, winClass, w, h)) return false;
	if (!gfx.Initialize(window.GetHandle(), w, h)) return false;
	return true;
}

bool Engine::ProcessMessages()
{
	return window.ProcessMessages();
}

void Engine::Awake()
{
}

void Engine::Start()
{
	//keyboard.EnableAutoRepeatKeys();
	//keyboard.DisableAutoRepeatKeys();
	keyboard.EnableAutoRepeatChars();
	//keyboard.DisableAutoRepeatChars();
}

void Engine::Update()
{
	time.UpdateDeltaTime();
	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent ke = keyboard.ReadKey();
		unsigned char key = ke.GetKeyCode();
	}

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char key = keyboard.ReadChar();
	}
}

void Engine::Render()
{
	gfx.RenderFrame();
}
