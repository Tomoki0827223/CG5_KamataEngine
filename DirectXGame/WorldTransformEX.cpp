#include "WorldTransformEX.h"

using namespace KamataEngine;
using namespace KamataEngine::MathUtility;

// World行列を更新
void WorldTransformEX::UpdateWorldMatrix() {
	matWorld_ = MakeAffineMatrix();
	TransferMatrix();
}

// アフィン変換行列を作成
KamataEngine::Matrix4x4 WorldTransformEX::MakeAffineMatrix() {
	// スケール行列
	Matrix4x4 matScale = MakeScaleMatrix(scale_);
	// 回転行列
	Matrix4x4 matRotX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;
	// 平行移動行列
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);
	// ワールド行列
	Matrix4x4 matWorld = matScale * matRot * matTrans;
	return matWorld;
}