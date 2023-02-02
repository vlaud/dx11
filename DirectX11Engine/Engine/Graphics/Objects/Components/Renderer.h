#pragma once
#include "Component.h"
#include "../Mesh.h"
#include "../../ConstantBufferTypes.h"

class Renderer : public Component
{
protected:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* dc = nullptr;
	ConstantBuffer<CB_VS_WVP>* cb_mat = nullptr;
	string directory = "";
	virtual bool LoadFile(const string& filePath) = 0;
public:
	virtual void Update() override = 0;
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* dc,
		ID3D11ShaderResourceView* tex, ConstantBuffer<CB_VS_WVP>& cb_mat);
	bool Initialize(const string& filePath, ID3D11Device* device,
		ID3D11DeviceContext* dc, ConstantBuffer<CB_VS_WVP>& cb_mat);
	virtual void Draw(const XMMATRIX vpMat) = 0;

};

