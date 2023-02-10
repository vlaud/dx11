#pragma once
#include "../Vertex.h"
#include "../Buffers.h"
#include <vector>
#include "Texture.h"
//for Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../../ComException.h"

template<typename T> //클래스 템플릿은 헤더에다 정의
class Mesh
{
	VertexBuffer<T> vb;
	IndexBuffer ib;
	ID3D11DeviceContext* dc;
	vector<Texture> textures;
	XMMATRIX local;
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* dc, vector<T>& vertices,
		vector<DWORD>& indices, vector<Texture>& textures, const XMMATRIX& mat)
	{
		this->dc = dc;
		this->textures = textures;
		local = mat;
		try
		{
			HRESULT hr = vb.Initialize(device, vertices.data(), vertices.size());
			COM_ERROR(hr, L"버텍스 버퍼 생성에 실패 하였습니다.");
			hr = ib.Initialize(device, indices.data(), indices.size());
			COM_ERROR(hr, L"인덱스 버퍼 생성에 실패 하였습니다.");
		}
		catch (ComException& ex)
		{
			Debug::Log(ex.what());
		}
	}
	Mesh(const Mesh& mesh)
	{
		dc = mesh.dc;
		vb = mesh.vb;
		ib = mesh.ib;
		textures = mesh.textures;
		local = mesh.local;
	}
	void Draw()
	{
		UINT offset = 0;
		for (Texture tex : textures)
		{
			if (tex.GetType() == aiTextureType::aiTextureType_DIFFUSE)
			{
				dc->PSSetShaderResources(0, 1, tex.GetTextureResourceViewAddress());
				break;
			}
		}
		dc->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.StridePtr(), &offset);
		dc->IASetIndexBuffer(ib.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		dc->DrawIndexed(ib.Size(), 0, 0);
	}
	
	void DrawWireframeAt(UINT faceNum, const vector<Vertex>& vertices, const vector<DWORD>& indices,
		ID3D11RasterizerState* Default, ID3D11RasterizerState* Wire)
	{
		UINT offset = 0;
		for (Texture tex : textures)
		{
			if (tex.GetType() == aiTextureType::aiTextureType_DIFFUSE)
			{
				dc->PSSetShaderResources(0, 1, tex.GetTextureResourceViewAddress());
				break;
			}
		}
		dc->IASetVertexBuffers(0, 1, vb.GetAddressOf(), vb.StridePtr(), &offset);
		dc->IASetIndexBuffer(ib.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		UINT faceCount = 0;
		for (UINT i = 0; i < indices.size(); i += 3)
		{
			if (faceCount == faceNum)
			{
				//draw wireframe
				dc->RSSetState(Wire);
			}
			else
			{
				dc->RSSetState(Default);
			}
			dc->DrawIndexed(3, i, 0);
			++faceCount;
		}
		
	}

	const XMMATRIX& GetLocal() const
	{
		return local;
	}
};

