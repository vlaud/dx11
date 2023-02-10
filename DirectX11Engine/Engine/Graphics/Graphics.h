#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Buffers.h"
#include "ConstantBufferTypes.h"
//for Sprite font
#include <SpriteBatch.h>
#include <SpriteFont.h>
//imGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "../Scenes/Scene.h"

class Graphics
{
	ComPtr<ID3D11Device> device; //그래픽카드
	ComPtr<ID3D11DeviceContext> dc; //렌더링 파이프라인
	ComPtr<IDXGISwapChain> swapChain; //이중 버퍼링
	ComPtr<ID3D11RenderTargetView> renderTargetView; //출력대상
	
	//Depth Stencil
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<ID3D11Texture2D> dsBuffer;
	ComPtr<ID3D11DepthStencilState> dsState;
	ComPtr<ID3D11DepthStencilState> dsDrawMask;
	ComPtr<ID3D11DepthStencilState> dsApplyMask;
	//Blend State
	ComPtr<ID3D11BlendState> blendState;

	ComPtr<ID3D11SamplerState> samp;
	//for Sprite Font
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<SpriteFont> spriteFont;

	unique_ptr<Scene> scene;

	bool InitializeDirectX(HWND hWnd, int w, int h);
	bool InitializeShaders();
	bool InitializeScene();

public:
	ComPtr<ID3D11RasterizerState> rsCullBack;
	ComPtr<ID3D11RasterizerState> rsCullFront;
	ComPtr<ID3D11RasterizerState> rsWire;

	ConstantBuffer<CB_VS_WVP> cb;
	ConstantBuffer<CB_VS_Matrix> cb2D;
	ConstantBuffer<CB_PS_Alpha> cb_alpha;
	ConstantBuffer<CB_PS_Light> cb_light;
	ConstantBuffer<CB_VS_CameraPos> cb_Campos;
	
	bool Initialize(HWND hWnd, int w, int h);
	void RenderFrame();
};

