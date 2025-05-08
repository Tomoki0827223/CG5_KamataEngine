#include "Shader.h"
#include <d3dcompiler.h>
// D3DCompileFromFile
#include <cassert>
// assert

// シェーダーファイルを読み込み、コンパイルする
void Shader::Load(const std::wstring& filePath, const std::string& shaderModel) {
    ID3DBlob* shaderBlob = nullptr; // 修正: ポインタ型に変更し、nullptr を正しく初期化
    ID3DBlob* errorBlob = nullptr;  // 修正: 正しい宣言に変更

    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(), // シェーダーファイル名
        nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
        "main", shaderModel.c_str(), // エントリーポイント名、シェーダモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
        0, &shaderBlob, &errorBlob // 修正: ポインタのアドレスを渡す
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            // エラーメッセージを出力
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        assert(false && "Shader compilation failed.");
    }

    // shaderBlob のリソース解放
    if (shaderBlob) {
        shaderBlob->Release();
    }
}