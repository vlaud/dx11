#include "Graphics.h"
#include <WICTextureLoader.h>
#include "../../RenderWindow.h"
#include "../Time.h"
#include "../Input/Input.h"

#include "ShaderHelper.h"
unordered_map<VS_SHADER, shared_ptr<VertexShader>> ShaderHelper::vslist;
unordered_map<PS_SHADER, shared_ptr<PixelShader>> ShaderHelper::pslist;

//#include "../Scenes/StudyBumpMap.h"
//#include "../Scenes/StudySpecular.h"
//#include "../Scenes/StudyPicking.h"
#include "../Scenes/StudyCollision.h"

//렌더링파이프라인
//IA 인풋어셈블러 - 완료 Input Layout
//VS 버텍스 셰이더 - 완료
//RS 레스터라이저 스테이트- 완료
//PS 픽셀 셰이더- 완료
//OM 아웃풋 머저 - 완료

bool Graphics::InitializeDirectX(HWND hWnd, int w, int h)
{
	vector<AdapterData> adapters = AdapterReader::GetAdapters();
	if (adapters.size() == 0)
	{
		Debug::Log(L"그래픽 카드를 발견하지 못했습니다.");
		return false;
	}
	try
	{
		//create Swapchain
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferDesc.Width = w;
		scd.BufferDesc.Height = h;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hWnd;
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			adapters[0].pAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			0, 0, 0, 0,
			D3D11_SDK_VERSION,
			&scd, swapChain.GetAddressOf(),
			device.GetAddressOf(),
			NULL,
			dc.GetAddressOf()
		);
		COM_ERROR(hr, L"DirectX 초기화에 실패 하였습니다.");

		ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR(hr, L"후면 버퍼를 가져오지 못했습니다.");

		hr = device->CreateRenderTargetView(backBuffer.Get(), 0, renderTargetView.GetAddressOf());
		COM_ERROR(hr, L"Render Target View 생성에 실패 하였습니다.");

		//Raster Rizer
		//Create Viewport
		CD3D11_VIEWPORT vp(0.0f, 0.0f, w, h);
		dc->RSSetViewports(1, &vp);

		//Create Rasterizer State
		CD3D11_RASTERIZER_DESC rsDesc(D3D11_DEFAULT);
		hr = device->CreateRasterizerState(&rsDesc, rsCullBack.GetAddressOf());
		COM_ERROR(hr, L"래스터라이저 스테이트 생성에 실패 하였습니다.");

		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT; //D3D11_CULL_NONE 망토같은 특수한 예
		hr = device->CreateRasterizerState(&rsDesc, rsCullFront.GetAddressOf());
		COM_ERROR(hr, L"래스터라이저 스테이트 생성에 실패 하였습니다.");

		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		hr = device->CreateRasterizerState(&rsDesc, rsWire.GetAddressOf());
		COM_ERROR(hr, L"래스터라이저 스테이트 생성에 실패 하였습니다.");

		//Create Depth Stencil State
		CD3D11_DEPTH_STENCIL_DESC dsDesc(D3D11_DEFAULT);
		hr = device->CreateDepthStencilState(&dsDesc, dsState.GetAddressOf());
		COM_ERROR(hr, L"뎁스 스텐실 스테이트 생성에 실패 하였습니다.");
		//Create Depth Stencil Draw Mask
		dsDesc.DepthEnable = false;
		dsDesc.StencilEnable = true;

		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

		hr = device->CreateDepthStencilState(&dsDesc, dsDrawMask.GetAddressOf());
		COM_ERROR(hr, L"Draw Mask 생성에 실패 하였습니다.");

		//Create Depth Stencil Apply Mask
		dsDesc.DepthEnable = true;
		dsDesc.StencilEnable = true;

		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		hr = device->CreateDepthStencilState(&dsDesc, dsApplyMask.GetAddressOf());
		COM_ERROR(hr, L"Apply Mask 생성에 실패 하였습니다.");

		//Create Depth Stencil Buffer
		CD3D11_TEXTURE2D_DESC dsbDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, w, h);
		dsbDesc.MipLevels = 1;
		dsbDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

		hr = device->CreateTexture2D(&dsbDesc, nullptr, dsBuffer.GetAddressOf());
		COM_ERROR(hr, L"뎁스 스텐실 버퍼 생성에 실패 하였습니다.");

		//Create Depth Stencil View
		hr = device->CreateDepthStencilView(dsBuffer.Get(), nullptr, dsView.GetAddressOf());
		COM_ERROR(hr, L"뎁스 스텐실  뷰 생성에 실패 하였습니다.");

		dc->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), dsView.Get()); //아웃풋 머저

		//Create Sampler State
		CD3D11_SAMPLER_DESC samDesc(D3D11_DEFAULT);
		samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = device->CreateSamplerState(&samDesc, samp.GetAddressOf());
		COM_ERROR(hr, L"샘플러 스테이트 생성에 실패 하였습니다.");

		//Create Sprite Font
		spriteBatch = make_unique<SpriteBatch>(dc.Get());
		spriteFont = make_unique<SpriteFont>(device.Get(), L"Assets/Fonts/mySpritefont.sf");

		//Create Blend State
		CD3D11_BLEND_DESC bsDesc(D3D11_DEFAULT);
		D3D11_RENDER_TARGET_BLEND_DESC rtbDesc = { 0 };

		rtbDesc.BlendEnable = true;
		rtbDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA; //앞쪽 블렌드
		rtbDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA; //1 - 앞쪽 블렌드값
		rtbDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		rtbDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		bsDesc.RenderTarget[0] = rtbDesc;

		hr = device->CreateBlendState(&bsDesc, blendState.GetAddressOf());
		COM_ERROR(hr, L"Blend State 생성에 실패 하였습니다.");
	}
	catch (ComException& ex)
	{
		Debug::Log(ex.what());
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{
	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		HRESULT hr = cb.Initialize(device.Get(), dc.Get());
		COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");

		hr = cb2D.Initialize(device.Get(), dc.Get());
		COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");

		hr = cb_alpha.Initialize(device.Get(), dc.Get());
		COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");
		cb_alpha.data.alpha = 1.0f;

		hr = cb_light.Initialize(device.Get(), dc.Get());
		COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");
		cb_light.data.ambientColor = XMFLOAT3(1, 1, 1);
		cb_light.data.ambientStrength = 0.3f;
		cb_light.data.lightColor = XMFLOAT3(1, 1, 1);
		cb_light.data.lightStrength = 1.0f;
		cb_light.data.lightDir = XMFLOAT3(0, -1, 0);

		hr = cb_Campos.Initialize(device.Get(), dc.Get());
		COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");
	}
	catch (ComException& ex)
	{
		Debug::Log(ex.what());
		return false;
	}

	scene = make_unique<StudyCollision>();
	scene->Initialize(this, device.Get(), dc.Get());

	return true;
}

bool Graphics::Initialize(HWND hWnd, int w, int h)
{
	if (!InitializeDirectX(hWnd, w, h)) return false;
	if (!InitializeShaders()) return false;
	if (!InitializeScene()) return false;
	//Setup imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), dc.Get());
	ImGui::StyleColorsDark();
	return true;
}

void Graphics::RenderFrame()
{
	float bgColor[] = { 0,0,0,1 };
	dc->ClearRenderTargetView(renderTargetView.Get(), bgColor);
	dc->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	dc->OMSetDepthStencilState(dsState.Get(), 0);
	

	dc->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);//블렌드 처리는 아웃풋 머저 단계에서

	dc->PSSetSamplers(0, 1, samp.GetAddressOf());
	cb_alpha.Update();

	dc->PSSetConstantBuffers(0, 1, cb_alpha.GetAddressOf());
	dc->RSSetState(rsCullBack.Get());
	
	cb_light.Update();
	dc->PSSetConstantBuffers(1, 1, cb_light.GetAddressOf());

	scene->Update(Time::GetDeltaTime());

	dc->OMSetDepthStencilState(dsState.Get(), 0);

	scene->RenderFrame();

	//Calculate fps
	static int fpsCounter = 0;
	fpsCounter++;
	static float playTime = 0.0f;
	playTime += Time::GetDeltaTime();
	static wstring fpsStr = L"FPS: 0";
	if (playTime >= 1.0f)
	{
		playTime = 0.0f;
		fpsStr = L"FPS: " + to_wstring(fpsCounter);
		fpsCounter = 0;
	}

	//Draw text
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), fpsStr.c_str(),
		XMFLOAT2(0, 0), Colors::White, 0, XMFLOAT2(0, 0), XMFLOAT2(1, 1));
	spriteBatch->End();

	//Draw imGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	scene->OnGUI();

	//Draw Call imGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(1, 0); //swap front <-> back 수직동기화 1,0
}
