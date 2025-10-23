#pragma once

#define NOMINMAX
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <memory>
#include <vector>

#include "Line3dBase.h"

#include "ModelStruct.h"
#include "Vector3.h"
#include "Vector4.h"

using Microsoft::WRL::ComPtr;

/// <summary>
/// 3Dライン描画クラス
/// </summary>
class Line3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="positions"></座標のリストを入れる>
	void Initialize(const std::vector<Vector3>& positions);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

	/// <summary>
	/// マテリアルデータを作成
	/// </summary>
	void MakeMaterialData();

public:

	/// <summary>
	/// アクセッサ
	/// </summary>

	// マテリアルデータ取得
	Material& GetMaterial() { return materialData_; }

	// 座標設定
	void SetPositions(const std::vector<Vector3>& positions);
	// 色設定
	void SetColor(const Vector3& color);

	// ボックスを作成
	std::vector<Vector3> CreateBox(const Vector3& min, const Vector3& max);
	// 球を作成
	std::vector<Vector3> CreateSphere(const float& radius);
	// セグメントを作成
	std::vector<Vector3> CreateSegment(const Vector3& start, const Vector3& direction);
	// グリッドを作成
	std::vector<Vector3> CreateGrid(float cell, float size, float y = 0.0f);

private:

	/*==================== マテリアル ====================*/

	Material materialData_;

	uint32_t lineIndex_ = 0;

};

