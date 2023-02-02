#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include <wrl/client.h> //com�������̽� ����Ʈ����Ʈ
#include <vector>
#include "../ComException.h"
using namespace std;
using namespace Microsoft::WRL;

struct AdapterData
{
	IDXGIAdapter* pAdapter = nullptr; //����� = �׷���ī��(���ǻ�)
	DXGI_ADAPTER_DESC desc;
	AdapterData(IDXGIAdapter* pAdapter);
};
class AdapterReader
{
	static vector<AdapterData> adapters;
public:
	static vector<AdapterData> GetAdapters();
};

