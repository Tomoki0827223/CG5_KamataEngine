#include "PipelineState.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void PipelineState::Create(D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc) 
{ 
	DirectXCommon* dxcommon = DirectXCommon::GetInstance(); 
	
	ID3D12PipelineState* grahpicspipelineState = nullptr;
	HRESULT hr = dxcommon->GetDevice()->
		CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&grahpicspipelineState));

	assert(SUCCEEDED(hr));

	pipelineState_ = grahpicspipelineState;
}

// 生成した PipelineState を返す
ID3D12PipelineState* PipelineState::Get() 
{ 
	return pipelineState_; 
}

// コンストラクタ
PipelineState::PipelineState() {}

// デストラクタ
PipelineState::~PipelineState() {
	if (pipelineState_) {
		pipelineState_->Release();
		pipelineState_ = nullptr;
	}
}