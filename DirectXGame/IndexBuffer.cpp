#include "IndexBuffer.h"
#include "KamataEngine.h"

#include <d3dx12.h>
#include <cassert>

using namespace KamataEngine;


void IndexBuffer::Create(const UINT size, const UINT stride) 
{ 
	assert(stride == 2 || stride == 4); 
	DXGI_FORMAT format = (stride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	DirectXCommon* dxcommon = DirectXCommon::GetInstance();

	// vertexResourceの作成------------------------------------------------------------------------------------------------
	// 頂点データの用意
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの作成
	D3D12_RESOURCE_DESC indexResourceDesc{};
	indexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	indexResourceDesc.Width = size;
	// バッファの場合はこれらは１にする決まり
	indexResourceDesc.Height = 1;
	indexResourceDesc.DepthOrArraySize = 1;
	indexResourceDesc.MipLevels = 1;
	indexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	indexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作成
	ID3D12Resource* indexResource = nullptr;
	HRESULT hr = dxcommon->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &indexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexResource));
	assert(SUCCEEDED(hr));

	indexBuffer_ = indexResource;

	// VERTEX_BUFFER_VIEWの作成@------------------------------------------------------------------------------------------------
	D3D12_INDEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = size;
	vertexBufferView.Format = format;
	indexBufferView_ = vertexBufferView;
}

ID3D12Resource* IndexBuffer::Get() 
{
	return indexBuffer_; 
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetView() 
{ 
	return &indexBufferView_; 
}

IndexBuffer::IndexBuffer() 
{

}

IndexBuffer::~IndexBuffer() 
{ 
	if (indexBuffer_) {
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}