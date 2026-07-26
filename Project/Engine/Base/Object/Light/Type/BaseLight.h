#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include "Vector3.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

namespace NumaEngine { class DirectXEngine; }

/// <summary>
/// ライトの基底クラス
/// </summary>
class BaseLight
{
public:

    // 初期化
	virtual void Initialize(NumaEngine::DirectXEngine* dxEngine) = 0;

	// 更新
	virtual void Update() = 0;

	// デバッグ用のImGui表示
	virtual void Debug_ImGui() = 0;

	// ライト用のデータ作成
	virtual void MakeLightData() = 0;

	// ライト用の行列作成
	virtual void BuildMatricesCoverAll(
		const NumaEngine::Vector3& sceneMin,
		const NumaEngine::Vector3& sceneMax,
		uint32_t shadowW, uint32_t shadowH) = 0;

	// バッファリソースの取得
	ID3D12Resource* GetResource()const { return resource_.Get(); }

	/// <summary>
	/// ライト用の行列構造体
	/// </summary>
	struct LightMatrix {
        NumaEngine::Matrix4x4 lightView;
		NumaEngine::Matrix4x4 lightProj;
		NumaEngine::Matrix4x4 lightVP;
	};
	const LightMatrix& GetLightMatrix() const { return lightMatrixs_; }
    const NumaEngine::Matrix4x4& GetLightVP()     const { return lightMatrixs_.lightVP; }

protected:

	/*==================== ライト用のデータ ====================*/

    // Engineのポインタ
	NumaEngine::DirectXEngine* dxEngine_ = nullptr;
	// バッファリソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	D3D12_VERTEX_BUFFER_VIEW bufferView_{};

	LightMatrix lightMatrixs_{};

};

