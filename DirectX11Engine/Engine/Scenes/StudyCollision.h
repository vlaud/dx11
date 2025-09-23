#pragma once
#include "Scene.h"
class StudyCollision : public Scene
{
	GameObject mainCam;
	GameObject model;

	GameObject box1, box2;

	bool boxCrash = false;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	vector<unique_ptr<GameObject>> obstacles;
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
	void OnText(unique_ptr<SpriteBatch>& spriteBatch)override;
	int GetScore() override;
};

