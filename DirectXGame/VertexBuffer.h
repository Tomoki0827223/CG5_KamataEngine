#pragma once
#include <d3dx12.h>

class VertexBuffer {

public:
	
	// VertexBuffer生成
	void Create(const UINT size, const UINT stride);
	
	// ゲッター
	// 頂点バッファー
	ID3D12Resource* Get();
	// 頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW* GetView();
	

	VertexBuffer();
	~VertexBuffer();

private:
	
	ID3D12Resource* vertexBuffer_ = nullptr; // 頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {}; // 頂点バッファービュー

};
