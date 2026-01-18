#pragma once

#include <memory>

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

#include "objects/fieldObject/effect/FieldObjectEffect.h"
#include "objects/fieldObject/adjustItem/FieldObjectAdjustItem.h"

/// 前方宣言
class MapCollision;
class GameCamera;

/// <summary>
/// フィールドオブジェクトのクラス
/// </summary>
class FieldObject : public Object3d,Collider
{
public:

	/// <summary>
	/// フィールドオブジェクトを初期化する
	/// </summary>
	/// <param name="object"></Jsonの保存データを取得する>
	void Init(SceneObject object);

	/// <summary>
	/// フィールドオブジェクトを更新する
	/// </summary>
	void Update();

	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <param name="items"></param>
	void SetItems(FieldObjectAdjustItem* items) { items_ = items; }

	/// <summary>
	/// マップの衝突判定を取得する
	/// </summary>
	/// <param name="mapCollision"></param>
	void SetMapCollision(MapCollision* mapCollision) { mapCollision_ = mapCollision; }

	/// <summary>
	/// カメラを取得する
	/// </summary>
	/// <param name="gameCamera"></param>
	void SetGameCamera(GameCamera* gameCamera) { gameCamera_ = gameCamera; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	/// <summary>
	/// 指定した範囲の乱数を取得する
	/// </summary>
	/// <param name="value"></指定する範囲>
	/// <returns></returns>
	float RandomRange(float value);

	/// <summary>
	/// シェイクの更新
	/// </summary>
	/// <param name="shake"></param>
	void UpdateShake(Vector3& shake);

	/// <summary>
	/// Objectが倒れる処理
	/// </summary>
	void UpdateBreak();

private:

	// 調整項目
	FieldObjectAdjustItem* items_ = nullptr;
	// マップの衝突判定
	MapCollision* mapCollision_ = nullptr;
	// カメラ
	GameCamera* gameCamera_ = nullptr;
	// エフェクト
	std::unique_ptr<FieldObjectEffect> effect_ = nullptr;

	// そのObjectが倒れたか
	bool isBreak_ = false;
	float breakTimer_ = 0.0f;
	Quaternion prevRotate_ = Quaternion::IdentityQuaternion();
	Quaternion breakRotate_ = Quaternion::IdentityQuaternion();
	// シェイクしている時値が入っている
	Vector3 shake_;
	// オブジェクトの元の座標
	Vector3 centerPosition_;

	float alpha_ = 1.0f;
	bool isGround_ = false;


};

