#pragma once

#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

using Microsoft::WRL::ComPtr;

namespace NumaEngine { class Model; }

/// <summary>
/// ワールド変換を扱うクラス
/// </summary>
class WorldTransform
{
public:

	/// <summary>
	/// ワールド変換行列
	/// </summary>
	struct TransformationMatrix {
        NumaEngine::Matrix4x4 WVP;
		NumaEngine::Matrix4x4 World;
		NumaEngine::Matrix4x4 WorldInverseTranspose;
	};
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	WorldTransform();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WorldTransform() = default;

    /// <summary>
	/// モデルからワールド変換行列を転送
	/// </summary>
	/// <param name="model"></param>
	void TransferMatrix(const NumaEngine::Model* model);
void TransferMatrix(const NumaEngine::Matrix4x4& localMatrix);

public:

    // ローカルスケール
	NumaEngine::Vector3 scale_ = { 1, 1, 1 };
	// ローカル回転角
	NumaEngine::Quaternion rotation_ = NumaEngine::Quaternion::IdentityQuaternion();
	// ローカル座標
	NumaEngine::Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
    NumaEngine::Matrix4x4 matWorld_ = NumaEngine::Matrix4x4::Identity();
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;
	// Instance描画用行列
	TransformationMatrix instanceMatrix_;


private:

};


