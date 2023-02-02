#include "AdapterReader.h"

vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&desc);
	if (FAILED(hr))
	{
		Debug::Log(hr, L"����� ������ �о� ���� ���߽��ϴ�");
	}
}

vector<AdapterData> AdapterReader::GetAdapters()
{
	if (adapters.size() > 0) return adapters;

	ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		Debug::Log(hr, L"DXGIFactory ������ �����Ͽ����ϴ�.");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT idx = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(idx++, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
	}
	return adapters;
}
