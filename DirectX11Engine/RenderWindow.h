#pragma once
#include "Engine/Debug.h"

class WindowContainer;

class RenderWindow
{
	HWND handle = nullptr;
	HINSTANCE hInst = nullptr;
	wstring Title = L"";
	wstring winClass = L"";
	void RegisterWindowClass();
public:
	static int ScreenWidth;
	static int ScreenHeight;
	bool Initialize(WindowContainer* pWindow, HINSTANCE hInst, wstring title, wstring winClass, int w, int h);
	bool ProcessMessages();
	~RenderWindow();
	HWND GetHandle();
};

