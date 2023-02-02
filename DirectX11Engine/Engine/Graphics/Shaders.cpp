#include "Shaders.h"
#include "../Debug.h"
bool VertexShader::Initialize(ID3D11Device* device, wstring path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements)
{
    HRESULT hr = D3DReadFileToBlob(path.c_str(), buffer.GetAddressOf());
    if (FAILED(hr))
    {
        wstring msg = L"셰이더를 읽어 오지 못했습니다. \n";
        msg += L"path : ";
        msg += path;
        Debug::Log(hr, msg);
        return false;
    }
    hr = device->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(),
        nullptr, shader.GetAddressOf());
    if (FAILED(hr))
    {
        wstring msg = L"버텍스 셰이더 생성에 실패 하였습니다. \n";
        msg += L"path : ";
        msg += path;
        Debug::Log(hr, msg);
        return false;
    }
    hr = device->CreateInputLayout(layout, numElements, buffer->GetBufferPointer(),
        buffer->GetBufferSize(), inputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        Debug::Log(hr, L"Input Layout 생성에 실패 하였습니다. \n");
        return false;
    }

    return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
    return shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
    return buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
    return inputLayout.Get();
}

bool PixelShader::Initialize(ID3D11Device* device, wstring path)
{
    HRESULT hr = D3DReadFileToBlob(path.c_str(), buffer.GetAddressOf());
    if (FAILED(hr))
    {
        wstring msg = L"셰이더를 읽어 오지 못했습니다. \n";
        msg += L"path : ";
        msg += path;
        Debug::Log(hr, msg);
        return false;
    }
    hr = device->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(),
        nullptr, shader.GetAddressOf());
    if (FAILED(hr))
    {
        wstring msg = L"픽셀 셰이더 생성에 실패 하였습니다. \n";
        msg += L"path : ";
        msg += path;
        Debug::Log(hr, msg);
        return false;
    }

    return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
    return shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
    return buffer.Get();
}
