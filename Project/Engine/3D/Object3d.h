#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
#include <memory>
#include <string>

#include "Object3dBase.h"
#include "WorldTransform.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "ModelStruct.h"

using Microsoft::WRL::ComPtr;

class Model;

/// <summary>
/// 3Dオブジェクトクラス
/// </summary>
class Object3d
{
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filePath"></描画したい3Dオブジェクトのファイルパス>
	void Initialize(const std::string& filePath);
	
	/// <summary>
	/// シーンレンダラーを設定
	/// </summary>
	void SetSceneRenderer();

	/// <summary>
	/// レンダラーを削除
	/// </summary>
	void RemoveRenderer();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アクセッサ
	/// </summary>

	// モデルの設定
	void SetModel(const std::string& filePath);
	// テクスチャの設定
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	// 環境マップテクスチャの設定
	void SetTexture_ENV(const std::string& directoryPath, const std::string& filePath);
	// カラーの設定
	void SetColor(const Vector4& color);
	// ワールド変換行列の取得
	const WorldTransform& GetTransform() { return transform_; }
	// ワールド変換行列の設定
	void SetTransform(const WorldTransform& transform) { transform_ = transform; }
	void SetTransformParent(const WorldTransform* parent) { transform_.parent_ = parent; }
	void SetTransformScale(const Vector3& scale) { transform_.scale_ = scale; }
	void SetTransformRotation(const Quaternion& rotation) { transform_.rotation_ = rotation; }
	void SetTransformTranslation(const Vector3& translation) { transform_.translation_ = translation; }
	// モデルの取得
	Model* GetModel() { return model_; }
	// マテリアルの取得
	Material& GetMaterial() { return materialData_; }
	// カラーの取得
	Vector4& GetColor() { return materialData_.color; }

private:

	// マテリアルデータの作成
	void MakeMaterialData();

protected:

	WorldTransform transform_;

private:

	std::unique_ptr<Object3dBase> object3dBase_ = nullptr;

	Model* model_ = nullptr;

	/*==================== マテリアル ====================*/

	Material materialData_;

};