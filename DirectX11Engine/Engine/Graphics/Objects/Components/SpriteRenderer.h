#pragma once
#include "Component.h"
#include "../Texture.h"
#include "../../ConstantBufferTypes.h"
#include "../../Buffers.h"
#include "../../Vertex.h"
#include <memory>
using namespace std;

class SpriteRenderer : public Component
{
	unique_ptr<Texture> texture;
	ID3D11DeviceContext* dc = nullptr;
	ConstantBuffer<CB_VS_Matrix>* cb;
	VertexBuffer<Vertex2D> vb;
	IndexBuffer ib;
	float width, height;
public:
	void Update() override;
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* dc, 
		float w, float h, string path, ConstantBuffer<CB_VS_Matrix>& cb);
	void Draw(XMMATRIX ortho);
	float GetWidth();
	float GetHeight();
	void SetTransform();
};

