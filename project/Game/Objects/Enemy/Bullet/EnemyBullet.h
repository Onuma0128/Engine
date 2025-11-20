#pragma once
#include <functional>

#include "Object3d.h"
#include "Collider.h"

#include "objects/enemy/type/EnemyType.h"

class EnemyAdjustItem;

/// <summary>
/// 敵の弾クラス
/// </summary>
class EnemyBullet : public Object3d,Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="colliderName"></弾のColliderNameを入れる>
	/// <param name="type"></敵のタイプを入れる>
	void Init(const std::string& colliderName, EnemyType type);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 当たったタイミングで返す
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other)override;

	/// <summary>
	/// 弾を撃つ関数
	/// </summary>
	/// <param name="transform"></撃つ敵のTransformを入れる>
	void Attack(const WorldTransform& transform);


	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <returns></returns>
	// 弾が動いているか
	bool GetIsActive()const { return isActive_; }
	// isActiveがfalseになった瞬間のコールバック関数
	void SetOnDeactivateCallback(const std::function<void()>& callback);
	// 調整項目を取得する
	void SetItem(EnemyAdjustItem* item) { item_ = item; }

private:

	/// <summary>
	/// 弾が何かしらに当たっているなら呼び出す
	/// </summary>
	void IsCollision();

	/// <summary>
	/// 敵のタイプによって弾の速度を変化させ取得する
	/// </summary>
	/// <returns></弾の速度>
	const float GetTypeBulletSpeed();

private:

	// 調整項目
	EnemyAdjustItem* item_ = nullptr;
	// 敵のタイプを取得する
	EnemyType type_;

	// 今動いているか
	bool isActive_ = false;
	// 前フレームのisActive
	bool wasActive_ = false;
	// コールバック関数
	std::function<void()> onDeactivatedCallback_;

	// 速度
	Vector3 velocity_;
	// 動いている時間
	float activeFrame_ = 0.0f;

};

