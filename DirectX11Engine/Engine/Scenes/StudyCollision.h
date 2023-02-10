#pragma once
#include "Scene.h"
class StudyCollision : public Scene
{
	GameObject mainCam;
	GameObject model;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	vector<unique_ptr<GameObject>> obstacles;
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
};

