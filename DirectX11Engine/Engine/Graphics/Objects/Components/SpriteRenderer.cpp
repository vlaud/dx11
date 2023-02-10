#include "SpriteRenderer.h"
#include "../../../ComException.h"
#include "../GameObject.h"
#include "ComponentTypes.h"

void SpriteRenderer::Update()
{
}

bool SpriteRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* dc, float w, float h,
	string path, ConstantBuffer<CB_VS_Matrix>& cb)
{
	width = w;
	height = h;
	this->dc = dc;
	this->cb = &cb;
	texture = make_unique<Texture>(device, path, aiTextureType::aiTextureType_DIFFUSE);

	Vertex2D vertices[] =
	{
		{-0.5, -0.5f, 0.0f, 0, 0},
		{0.5, -0.5f, 0.0f, 1, 0},
		{-0.5, 0.5f, 0.0f, 0, 1},
		{0.5, 0.5f, 0.0f, 1, 1},
	};
	DWORD indices[] =
	{
		0,1,2,
		1,3,2,
	};
	try
	{
		HRESULT hr = vb.Initialize(device, vertices, ARRAYSIZE(vertices));
		COM_ERROR(hr, L"버텍스 버퍼 생성에 실패 하였습니다.");

		hr = ib.Initialize(device, indices, ARRAYSIZE(indices));
		COM_ERROR(hr, L"인덱스 버퍼 생성에 실패 하였습니다.");
	}
	catch (ComException& ex)
	{
		Debug::Log(ex.what());
		return false;
	}

	return true;
}


void SpriteRenderer::Draw(XMMATRIX ortho)
{
	cb->data.mat = gameObject->GetTransform()->GetWorld() * ortho;
	cb->Update();
	dc->VSSetConstantBuffers(0, 1, cb->GetAddressOf());

	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.StridePtr(), &offset);
	dc->IASetIndexBuffer(ib.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	dc->PSSetShaderResources(0, 1, texture->GetTextureResourceViewAddress());
	dc->DrawIndexed(ib.Size(), 0, 0);
}

float SpriteRenderer::GetWidth()
{
	return width;
}

float SpriteRenderer::GetHeight()
{
	return height;
}

void SpriteRenderer::SetTransform()
{
	gameObject->GetTransform()->SetPos(0, 0, 0);
	gameObject->GetTransform()->SetRot(0, 0, 0);
	gameObject->GetTransform()->SetScale(width, height, 1);
}
