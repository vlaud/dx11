#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "../Debug.h"
using namespace Microsoft::WRL;

class Buffer
{
protected:
	ComPtr<ID3D11Buffer> buffer;
	UINT size = 0;
public:
	ID3D11Buffer* Get() const
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();
	}
	UINT Size() const
	{
		return size;
	}
};
template<typename T>
class VertexBuffer : public Buffer
{
	UINT stride = 0;
public:
	VertexBuffer() {};
	HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices)
	{
		if (buffer.Get() != nullptr) buffer.Reset();

		size = numVertices;
		stride = sizeof(T);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.ByteWidth = sizeof(T) * numVertices;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbData = { 0 };
		vbData.pSysMem = data;
		return device->CreateBuffer(&vbDesc, &vbData, buffer.GetAddressOf());
	}
	const UINT Stride() const
	{
		return stride;
	}
	const UINT* StridePtr() const
	{
		return &stride;
	}
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer() {};
	HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT numVertices)
	{
		if (buffer.Get() != nullptr) buffer.Reset();

		size = numVertices;
		
		D3D11_BUFFER_DESC ibDesc = { 0 };
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.ByteWidth = sizeof(DWORD) * numVertices;
		ibDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibData = { 0 };
		ibData.pSysMem = data;

		return device->CreateBuffer(&ibDesc, &ibData, buffer.GetAddressOf());
	}
};

template<typename T>
class ConstantBuffer : public Buffer
{
	ID3D11DeviceContext* dc = nullptr;

public:
	T data;
	ConstantBuffer() {};
	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* dc)
	{
		if (buffer.Get() != nullptr) buffer.Reset();
		this->dc = dc;

		D3D11_BUFFER_DESC cbDesc = { 0 };
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.ByteWidth = sizeof(T) + (16 - sizeof(T) % 16); //16바이트 단위로 고정
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		
		return device->CreateBuffer(&cbDesc, 0, buffer.GetAddressOf());
	}
	bool Update()
	{
		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = dc->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map); //lock
		if (FAILED(hr))
		{
			Debug::Log(hr, L"컨스턴트 버퍼를 사용 할 수 없습니다.");
			return false;
		}
		CopyMemory(map.pData, &data, sizeof(data));
		dc->Unmap(buffer.Get(), 0);//unlock
		return true;
	}
};