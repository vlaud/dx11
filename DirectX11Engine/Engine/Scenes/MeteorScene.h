#pragma once
#include "Scene.h"
class MeteorScene : public Scene
{
	GameObject mainCam;
	GameObject campos;
	unique_ptr<GameObject> model;
	unique_ptr<SpriteFont> spriteFont;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	vector<Vertex> vertices;
	vector<DWORD> indices;

	vector<unique_ptr<GameObject>> obstacles;

	float pickDist = 0.0f;
	int score = 0;
	int life = 3;

	bool SphereIntersect(XMVECTOR orgDir, XMVECTOR rayDir);
	void Picking();
	bool IsPickingObs(unique_ptr<GameObject>& obj);
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
	void OnText(unique_ptr<SpriteBatch>& spriteBatch) override;
	int GetScore() override;
};

