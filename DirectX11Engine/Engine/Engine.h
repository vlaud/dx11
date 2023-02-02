#pragma once
#include "../WindowContainer.h"
#include "Graphics/Graphics.h"
#include "Time.h"
class Engine : public WindowContainer
{
	Graphics gfx;
	Time time;
public:
	bool Initialize(HINSTANCE hInst, wstring title, wstring winClass, int w, int h);
	bool ProcessMessages();
	void Awake() override;
	void Start();
	void Update();
	void Render();
};

