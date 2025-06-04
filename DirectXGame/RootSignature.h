#pragma once

#include <d3d12.h>

class RootSignature 
{
public:

	void Createe();

	ID3D12RootSignature* Get();

	RootSignature();

	~RootSignature();

private:

	ID3D12RootSignature* rootsignature_ = nullptr;


};
