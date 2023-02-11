#pragma once
#include "../Graphics/Objects/GameObject.h"
#include "../Graphics//ShaderHelper.h"
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex;
class Graphics;
class ID3D11Device;
class ID3D11DeviceContext;

class Scene
{
protected:
	Graphics* gfx = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* dc = nullptr;
	virtual bool InitializeScene() = 0;
public:
	bool Initialize(Graphics* gfx, ID3D11Device* device, ID3D11DeviceContext* dc)
	{
		this->gfx = gfx;
		this->device = device;
		this->dc = dc;
		return InitializeScene();
	}
	virtual void Update(float delta) = 0;
	virtual void RenderFrame() = 0;
	virtual void OnGUI() = 0;

};