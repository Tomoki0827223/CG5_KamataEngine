#include <Windows.h>
#include "KamataEngine.h"



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//初期化処理
	// // エンジンの初期化

	KamataEngine::Initialize(L"LE3C_26_ムラタ_トモキ");

	//メインループ
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}
	}

	// 終了処理
	KamataEngine::Finalize();
	return 0;
}
