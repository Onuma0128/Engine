#pragma once

#include <memory>

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

#include "objects/fieldObject/effect/FieldObjectEffect.h"

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

private:

	std::unique_ptr<FieldObjectEffect> effect_ = nullptr;

	// シェイクしている時値が入っている
	Vector3 shake_;
	// オブジェクトの元の座標
	Vector3 centerPosition_;


};

