#include "RootSignature.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void RootSignature::Createe() 
{
	if (rootsignature_) {
		rootsignature_->Release();
		rootsignature_ = nullptr;
	}
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// Rootsignatureの取得------------------------------------------------------------------------------------------------------------------------------
	// 構造体にデータを用意する
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlog = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlog);
	if (FAILED(hr)) {
		DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlog->GetBufferPointer()));
		assert(false);
	}

	// バイナリを基に作成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), 
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	signatureBlob->Release();

	rootsignature_ = rootSignature;
}

ID3D12RootSignature* RootSignature::Get() { return rootsignature_; }

RootSignature::RootSignature() {}

RootSignature::~RootSignature() 
{
	if (rootsignature_) {
		rootsignature_->Release();
		rootsignature_ = nullptr;
	}
}
