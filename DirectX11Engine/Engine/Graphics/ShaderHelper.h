#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "Shaders.h"
using namespace std;

enum class VS_SHADER
{
	DEFAULT,
	SKINNED,
	SPRITE
};
enum class PS_SHADER
{
	DEFAULT,
	SPRITE
};
static wstring GetShaderFolder()
{
	wstring shaderfolder = L"";
#ifdef _DEBUG
#ifdef _WIN64
	shaderfolder += L"x64/Debug/";
#else //x86
	shaderfolder += L"Debug/";
#endif
#else //Release
#ifdef _WIN64
	shaderfolder += L"x64/Release/";
#else //x86
	shaderfolder += L"Release/";
#endif
#endif
	return shaderfolder;
}

static wstring GetShaderPath(VS_SHADER type)
{
	wstring path = GetShaderFolder();
	switch (type)
	{
	case VS_SHADER::DEFAULT:
		path += L"vs.cso";
		break;
	case VS_SHADER::SKINNED:
		path += L"vsAnimation.cso";
		break;
	case VS_SHADER::SPRITE:
		path += L"vsSprite.cso";
		break;
	}
	return path;
}
static wstring GetShaderPath(PS_SHADER type)
{
	wstring path = GetShaderFolder();
	switch (type)
	{
	case PS_SHADER::DEFAULT:
		path += L"ps.cso";
		break;
	case PS_SHADER::SPRITE:
		path += L"psSprite.cso";
		break;
	}
	return path;
}

static void GetInputLayout(VS_SHADER type, vector<D3D11_INPUT_ELEMENT_DESC>& layout)
{
	switch (type)
	{
	case VS_SHADER::DEFAULT:
		layout.push_back({ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		break;
	case VS_SHADER::SKINNED:
		layout.push_back({ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "boneIDs", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "weights", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
	D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		break;
	case VS_SHADER::SPRITE:
		layout.push_back({ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout.push_back({ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 });
		break;
	}
}

class ShaderHelper
{
	static unordered_map<VS_SHADER, shared_ptr<VertexShader>> vslist;
	static unordered_map<PS_SHADER, shared_ptr<PixelShader>> pslist;
public:
	static VertexShader* GetShader(ID3D11Device* device, VS_SHADER type)
	{
		if (vslist.find(type) == vslist.end())
		{
			vslist[type] = make_shared<VertexShader>();
			vector<D3D11_INPUT_ELEMENT_DESC> layout;
			GetInputLayout(type, layout);
			vslist[type]->Initialize(device, GetShaderPath(type), layout.data(), layout.size());
		}
		return vslist[type].get();
	}
	static PixelShader* GetShader(ID3D11Device* device, PS_SHADER type)
	{
		if (pslist.find(type) == pslist.end())
		{
			pslist[type] = make_shared<PixelShader>();
			pslist[type]->Initialize(device, GetShaderPath(type));
		}
		return pslist[type].get();
	}
};