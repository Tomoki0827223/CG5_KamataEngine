#pragma once
#include "3d\WorldTransform.h"
#include "KamataEngine.h"

class WorldTransformEX : 
	public KamataEngine ::WorldTransform{

public:
	
	void UpdateWorldMatrix();

	KamataEngine::Matrix4x4 MakeAffineMatrix();

};
