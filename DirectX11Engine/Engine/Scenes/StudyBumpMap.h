#pragma once
#include "Scene.h"
#include "../Graphics/Objects/Texture.h"

struct VertexBump;

class StudyBumpMap : public Scene
{
	GameObject mainCam;
	GameObject model;

	VertexShader* vs = nullptr;
	PixelShader* ps = nullptr;
	unique_ptr<Texture> bumpTex;

	void CalculatorTangentBinormal(VertexBump& v1, VertexBump& v2, VertexBump& v3);
protected:
	bool InitializeScene() override;
public:
	void Update(float delta) override;
	void RenderFrame() override;
	void OnGUI() override;
};

