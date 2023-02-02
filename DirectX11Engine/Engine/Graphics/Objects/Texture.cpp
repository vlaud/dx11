#include "Texture.h"
#include "../../ComException.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
string GetFileExtension(const string& filename) // 확장자 리턴
{
    size_t off = filename.find_last_of('.');
    if (off == string::npos)
    {
        return "";
    }
    return string(filename.substr(off + 1));
}
void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const Color& color, aiTextureType type)
{
	InitializeColorTexture(device, &color, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const Color* color, UINT w, UINT h, aiTextureType type)
{
    this->type = type;
    CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_R8G8B8A8_UNORM, w, h);
    ID3D11Texture2D* p2DTex = nullptr;
    D3D11_SUBRESOURCE_DATA iniData = { 0 };
    iniData.pSysMem = color;
    iniData.SysMemPitch = w * sizeof(Color);

    try
    {
        HRESULT hr = device->CreateTexture2D(&desc, &iniData, &p2DTex);
        COM_ERROR(hr, L"컬러 텍스쳐 생성에 실패 하였습니다.");
        texture = static_cast<ID3D11Texture2D*>(p2DTex);
        CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, desc.Format);
        hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, texView.GetAddressOf());
        COM_ERROR(hr, L"Shader Resource View 생성에 실패 하였습니다.");
    }
    catch (ComException& ex)
    {
        Debug::Log(ex.what());
    }
}

Texture::Texture(ID3D11Device* device, const Color& color, aiTextureType type)
{
    Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, const Color* color, UINT w, UINT h, aiTextureType type)
{
    InitializeColorTexture(device, color, w, h, type);
}

Texture::Texture(ID3D11Device* device, const string& filePath, aiTextureType type)
{
    this->type = type;
    HRESULT hr = { 0 };
    try
    {
        if (GetFileExtension(filePath) == "dds")
        {
            hr = DirectX::CreateDDSTextureFromFile(device, wstring(filePath.begin(), filePath.end()).c_str(),
                texture.GetAddressOf(), texView.GetAddressOf());
            COM_ERROR(hr, L"dds 파일을 읽어 오지 못했습니다.");
        }
        else
        {
            hr = DirectX::CreateWICTextureFromFile(device, wstring(filePath.begin(), filePath.end()).c_str(),
                texture.GetAddressOf(), texView.GetAddressOf());
            COM_ERROR(hr, L"텍스쳐 파일을 읽어 오지 못했습니다.");
        }
    }
    catch (ComException& ex)
    {
        Debug::Log(ex.what());
        Initialize1x1ColorTexture(device, COLORS::UnLoadedTextureColor, type);
    }
}

Texture::Texture(ID3D11Device* device, const uint8_t* data, size_t size, aiTextureType type)
{
    this->type = type;
    try
    {
        HRESULT hr = DirectX::CreateWICTextureFromMemory(device, data, size,
            texture.GetAddressOf(), texView.GetAddressOf());
        COM_ERROR(hr, L"텍스쳐 생성을 실패했습니다.");
    }
    catch (ComException& ex)
    {
        Debug::Log(ex.what());
        Initialize1x1ColorTexture(device, COLORS::UnLoadedTextureColor, type);
    }
   
}

Texture::Texture(ID3D11ShaderResourceView* tex, aiTextureType type)
{
    this->type = type;
    texView = tex;
    
}

aiTextureType Texture::GetType()
{
    return type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
    return texView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
    return texView.GetAddressOf();
}
