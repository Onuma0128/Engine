#pragma once

#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

using Microsoft::WRL::ComPtr;

class Model;

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
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
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
	void TransferMatrix(const Model* model);
	void TransferMatrix(const Matrix4x4& localMatrix);

public:

	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// ローカル回転角
	Quaternion rotation_ = Quaternion::IdentityQuaternion();
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_ = Matrix4x4::Identity();
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;
	// Instance描画用行列
	TransformationMatrix instanceMatrix_;


private:

};