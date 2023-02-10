#pragma once
#include "Scene.h"
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex; 

class StudyPicking : public Scene
{
	GameObject mainCam;
	unique_ptr<GameObject> model;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;
	
	vector<Vertex> vertices;
	vector<DWORD> indices;

	int pickFace = -1;
	float pickDist = 0.0f;

	bool SphereIntersect(XMVECTOR orgDir, XMVECTOR rayDir);
	bool TriangleIntersect(XMVECTOR rayPos, XMVECTOR rayDir, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* t, float* u, float* v);
	void Picking();
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
};

