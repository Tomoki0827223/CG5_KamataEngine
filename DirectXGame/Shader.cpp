#include "Shader.h"
#include <d3dcompiler.h>
// D3DCompileFromFile
#include <cassert>
// assert

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel) {

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// wstring => string 文字列変換
	std::string mbShaderModel = miscUtility_->ConvertToString(shaderModel);

	HRESULT hr = D3DCompileFromFile(
	    filePath.c_str(),                                // シェーダーファイル名
	    nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,      // インクルード可能にする
	    "main",                                          // エントリーポイント名
	    mbShaderModel.c_str(),                           // シェーダーモデル（stringに変換して渡す）
	    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
	    0, &shaderBlob, &errorBlob);

	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		assert(false && "Shader compilation failed.");
	}

	blob = shaderBlob;
}

void Shader::LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel) 
{
	// DXCの初期化
	static IDxcUtils* dxcUtils = nullptr;
	static IDxcCompiler3* dxcCompiler = nullptr;
	static IDxcIncludeHandler* dxcIncludeHandler = nullptr;

	HRESULT hr;

	if (dxcUtils == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
	}
	if (dxcCompiler == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));
	}
	if (dxcIncludeHandler == nullptr) {
		hr = dxcUtils->CreateDefaultIncludeHandler(&dxcIncludeHandler);
		assert(SUCCEEDED(hr));
	}

	// 1. hlslファイルを読み込む
	IDxcBlobEncoding* shadersource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shadersource);
	assert(SUCCEEDED(hr));

	//読み込んだファイル内容をDxcBufferに設定する
	DxcBuffer shadersourceBuffer;
	shadersourceBuffer.Ptr = shadersource->GetBufferPointer();
	shadersourceBuffer.Size = shadersource->GetBufferSize();
	shadersourceBuffer.Encoding = DXC_CP_ACP;

	// 2. コンパイルオプションを設定する
	LPCWSTR arguments[] = {
	    filePath.c_str(), // シェーダーファイル名
	    L"-E",
	    L"main", // エントリーポイント名
	    L"-T",
	    shaderModel.c_str(), // シェーダーモデル
	    L"-Zi",              // デバッグ情報を生成
	    L"-Qembed_debug",    // デバッグ情報を埋め込む
	    L"-Od",              // デバッグ用設定
	    L"-Zpr",             // プリプロセッサの出力を生成
	};

	// 3. コンパイルを実行する
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
	    &shadersourceBuffer, // シェーダーソース
	    arguments,           // コンパイルオプション
	    _countof(arguments), // オプションの数
	    dxcIncludeHandler,   // インクルードハンドラ
	    IID_PPV_ARGS(&shaderResult));
	assert(SUCCEEDED(hr));

	// 警告やエラーがあれば表示する
	IDxcBlobUtf8* shadererrors = nullptr;
	IDxcBlobWide* nameBlob = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shadererrors), &nameBlob);
	if (shadererrors) {
		OutputDebugStringA((char*)shadererrors->GetBufferPointer());
		shadererrors->Release();
	}


}


// コンパイル済みデータを取得する
ID3DBlob* Shader::GetBlob() {
	return blob; // 修正: blob を返す
}

IDxcBlob* Shader::GetDxcBlob() { return nullptr; }

// コンストラクタ
Shader::Shader() {
	// コンストラクタの初期化処理
}

// デストラクタ
Shader::~Shader() {
	if (blob) {
		blob->Release(); // 修正: blob を解放
		blob = nullptr;  // 修正: nullptr に設定
	}
}