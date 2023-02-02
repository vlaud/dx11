#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>
#include "Color.h"
using namespace std;
using namespace Microsoft::WRL;

enum class TextureStorageType
{
	Invaild,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};
class Texture
{
	ComPtr<ID3D11Resource> texture = nullptr;
	ComPtr<ID3D11ShaderResourceView> texView = nullptr;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
	void Initialize1x1ColorTexture(ID3D11Device* device, const Color& color, aiTextureType type);
	void InitializeColorTexture(ID3D11Device* device, const Color* color, UINT w, UINT h, aiTextureType type);
public:
	Texture(ID3D11Device* device, const Color& color, aiTextureType type);
	Texture(ID3D11Device* device, const Color* color, UINT w, UINT h, aiTextureType type);
	Texture(ID3D11Device* device, const string& filePath, aiTextureType type);
	Texture(ID3D11Device* device, const uint8_t* data, size_t size, aiTextureType type);
	Texture(ID3D11ShaderResourceView* tex, aiTextureType type);
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
};

