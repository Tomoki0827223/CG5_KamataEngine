#pragma once
#include <string> // 修正: include文をクラス外に移動
#include <d3d12.h> // 修正: include文をクラス外に移動

class Shader {
public:
	// シェーダーファイルを読み込み、コンパイル済みデータを生成する
	void Load(const std::wstring& filePath, const std::string& shaderModel); // 修正: 不要な "Shader.n" を削除
	
	// 生成したコンパイル済みデータを取得する
	ID3DBlob* GetBlob();
	
	// コンストラクタ
	Shader();
	
	// デストラクタ
	~Shader();

private:

	ID3DBlob* blob = nullptr; // コンストラクタで初期化しなくていい ※C++11以降

};