#pragma once
#include "C:\Users\dscka\Documents\ProjectFile\CG5\CG5_KamataEngine\External\KamataEngine\include\3d\WorldTransform.h"
#include "KamataEngine.h"

class WorldTransformEX : 
	public KamataEngine ::WorldTransform{

public:
	
	void UpdateWorldMatrix();

	KamataEngine::Matrix4x4 MakeAffineMatrix();

};
