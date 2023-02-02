#pragma once
#include "Scene.h"

class DefaultScene : public Scene
{
	GameObject mainCam;
	GameObject model;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
};

