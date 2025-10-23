#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DirectXEngine;

/// <summary>
/// ライトの基底クラス
/// </summary>
class BaseLight
{
public:

	// 初期化
	virtual void Initialize(DirectXEngine* dxEngine) = 0;

	// 更新
	virtual void Update() = 0;

	// デバッグ用のImGui表示
	virtual void Debug_ImGui() = 0;

	// ライト用のデータ作成
	virtual void MakeLightData() = 0;

	// バッファリソースの取得
	ID3D12Resource* GetResource()const { return resource_.Get(); }

protected:

	/*==================== ライト用のデータ ====================*/

	// Engineのポインタ
	DirectXEngine* dxEngine_ = nullptr;
	// バッファリソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	D3D12_VERTEX_BUFFER_VIEW bufferView_{};

};