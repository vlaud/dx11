#include "Renderer.h"
#include "../../../ComException.h"
#include "../../Objects/GameObject.h"

bool Renderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* dc, ID3D11ShaderResourceView* tex, ConstantBuffer<CB_VS_WVP>& cb_mat)
{
	this->device = device;
    this->dc = dc;
    this->cb_mat = &cb_mat;

    return true;
}

bool Renderer::Initialize(const string& filePath, ID3D11Device* device, ID3D11DeviceContext* dc, ConstantBuffer<CB_VS_WVP>& cb_mat)
{
	this->device = device;
	this->dc = dc;
	this->cb_mat = &cb_mat;
	if (!LoadFile(filePath))
	{
		Debug::Log(L"�߸��� �����Դϴ�.");
		return false;
	}
	return true;
}