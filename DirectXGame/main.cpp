#include "Shader.h"
#include "KamataEngine.h"
#include "RootSignature.h"
#include <Windows.h>
#include <cassert>

using namespace KamataEngine;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化処理
	//  // エンジンの初期化

	KamataEngine::Initialize(L"LE3C_26_ムラタ_トモキ");

	// DirectXCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// DirectXcommonのクラスが管理してる、Windowの幅と高さを取得
	int32_t w = dxCommon->GetBackBufferWidth();
	int32_t h = dxCommon->GetBackBufferHeight();
	DebugText::GetInstance()->ConsolePrintf(std::format("Width: {}, height: {}\n", w, h).c_str());

	// DirectXCommonのクラスが管理してる、コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

	//// Rootsignatureの取得------------------------------------------------------------------------------------------------------------------------------
	RootSignature rs;
	rs.Createe();

	// InputLayoutの設定--------------------------------------------------------------------------------------------------------------------------------
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1]{};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendStateの設定--------------------------------------------------------------------------------------------------------------------------------
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasterizerStateの設定----------------------------------------------------------------------------------------------------------------------
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面(反時計回り)を科リングする
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 塗りつぶしモードをソリッドにする(ワイヤーフレームなら D3D12_Ftill_MOOE_WIREFRAME)
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	Shader vs;
	vs.LoadDxc(L"Resources/shaders/TestVS.hlsl", L"vs_6_0");
	assert(vs.GetDxcBlob() != nullptr);	

	Shader ps;
	ps.LoadDxc(L"Resources/shaders/TestPS.hlsl", L"ps_6_0");
	assert(ps.GetDxcBlob() != nullptr);

	// PSO(PipelineStateObject)の作成-----------------------------------------------------------------------------------------------------------------
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rs.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.VS = {vs.GetDxcBlob()->GetBufferPointer(), vs.GetDxcBlob()->GetBufferSize()};
	graphicsPipelineStateDesc.PS = {ps.GetDxcBlob()->GetBufferPointer(), ps.GetDxcBlob()->GetBufferSize()};
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。△形状を指定
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むか設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// PSOの作成
	ID3D12PipelineState* graphicspipelineState = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicspipelineState));
	if (FAILED(hr)) {
		DebugText::GetInstance()->ConsolePrintf(std::system_category().message(hr).c_str());
		assert(false);
	}
	// vertexResourceの作成------------------------------------------------------------------------------------------------
	// 頂点データの用意
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの作成
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(Vector4) * 3;
	// バッファの場合はこれらは１にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作成
	ID3D12Resource* vertexResource = nullptr;
	hr = dxCommon->GetDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));
	// VERTEX_BUFFER_VIEWの作成@------------------------------------------------------------------------------------------------
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	vertexBufferView.StrideInBytes = sizeof(Vector4);
	// 頂点リソースに書き込む
	Vector4* vertexData = nullptr;
	// リソースのマッピング
	hr = vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0] = {-0.5f, -0.5f, 0.0f, 1.0f}; // 上
	vertexData[1] = {0.0f, 0.5f, 0.0f, 1.0f};   // 右下
	vertexData[2] = {0.5f, -0.5f, 0.0f, 1.0f};  // 左下
	// アンマップ
	vertexResource->Unmap(0, nullptr);

	// メインループ
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		// 描画処理
		dxCommon->PreDraw();

		commandList->SetGraphicsRootSignature(rs.Get());
		commandList->SetPipelineState(graphicspipelineState);
		commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		commandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->DrawInstanced(3, 1, 0, 0);

		// 描画処理
		dxCommon->PostDraw();
	}

	// 解放処理
	vertexResource->Release();
	graphicspipelineState->Release();
	
	//signatureBlob->Release();
	//rootSignature->Release();

	// 終了処理
	KamataEngine::Finalize();
}