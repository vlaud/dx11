#pragma once
#include "RenderWindow.h"
#include "Engine/Input/KeyBoard.h"
#include "Engine/Input/Mouse.h"
class WindowContainer
{
protected:
	RenderWindow window;
	KeyBoard keyboard;
	Mouse mouse;
public:
	WindowContainer();
	LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParma);
	void virtual Awake() = 0;
};