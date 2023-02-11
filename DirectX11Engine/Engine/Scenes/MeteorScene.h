#pragma once
#include "Scene.h"

class MeteorScene : public Scene
{
	GameObject mainCam;
	GameObject campos;
	unique_ptr<GameObject> model;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;

	vector<Vertex> vertices;
	vector<DWORD> indices;

	vector<unique_ptr<GameObject>> obstacles;

	float pickDist = 0.0f;

	bool SphereIntersect(XMVECTOR orgDir, XMVECTOR rayDir);
	void Picking();
	bool IsPickingObs(unique_ptr<GameObject>& obj);
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
};

