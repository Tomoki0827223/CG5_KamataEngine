#include "Shader.h"
#include "KamataEngine.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <Windows.h>
#include <cassert>

using namespace KamataEngine;

// 関数プロトタイプ宣言
void SetupPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps);

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

	Shader vs;
	vs.LoadDxc(L"Resources/shaders/TestVS.hlsl", L"vs_6_0");
	assert(vs.GetDxcBlob() != nullptr);	

	Shader ps;
	ps.LoadDxc(L"Resources/shaders/TestPS.hlsl", L"ps_6_0");
	assert(ps.GetDxcBlob() != nullptr);

	PipelineState pipelineState;
	SetupPipelineState(pipelineState, rs, vs, ps);

	struct VertexData 
	{
		Vector4 position; // 頂点の位置
		Vector2 texcoord; // テクスチャ座標（今回は使用しないが、将来の拡張のために追加）
	};

	VertexData vertices[] = {
	    {{-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	    {{-1.0f, 3.0f, 0.0f, 1.0f},  {0.0f, -1.0f}},
	    {{3.0f, -1.0f, 0.0f, 1.0f},  {2.0f, 1.0f} },
	};

	// 頂点バッファの作成
	VertexBuffer vb;
	vb.Create(sizeof(vertices), sizeof(vertices[0]));

	VertexData* pGpuVertices = nullptr;
	vb.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pGpuVertices));
	for (int i = 0; i < _countof(vertices); i++) {
		pGpuVertices[i] = vertices[i];
	}
	vb.Get()->Unmap(0, nullptr);

	// インデックスバッファの作成
	uint16_t indices[] = {0, 1, 2};
	IndexBuffer ib;
	ib.Create(sizeof(indices), sizeof(indices[0]));

	uint16_t* pGpuIndices = nullptr;
	ib.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pGpuIndices));
	for (int i = 0; i < _countof(indices); i++) {
		pGpuIndices[i] = indices[i];
	}
	ib.Get()->Unmap(0, nullptr);

	// メインループ
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		// 描画処理
		dxCommon->PreDraw();

		commandList->SetGraphicsRootSignature(rs.Get());
		commandList->SetPipelineState(pipelineState.Get());
		commandList->IASetVertexBuffers(0, 1, vb.GetView());
		commandList->IASetIndexBuffer(ib.GetView());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//commandList->DrawInstanced(3, 1, 0, 0);
		commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		// 描画処理
		dxCommon->PostDraw();
	}

	// 終了処理
	KamataEngine::Finalize();
}

void SetupPipelineState(PipelineState& pipelineState, RootSignature& rs, Shader& vs, Shader& ps) 
{
	// InputLayoutの設定--------------------------------------------------------------------------------------------------------------------------------
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
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

	pipelineState.Create(graphicsPipelineStateDesc);
}
