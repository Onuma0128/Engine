#pragma once
#include <functional>

#include "Object3d.h"
#include "Collider.h"

#include "../type/EnemyType.h"

class EnemyAdjustItem;

class EnemyBullet : public Object3d,Collider
{
public:

	void Finalize();

	void Init(const std::string& colliderName, EnemyType type);

	void Update();

	void OnCollisionEnter(Collider* other)override;

	// 攻撃をする関数
	void Attack(const WorldTransform& transform);

	bool GetIsActive()const { return isActive_; }

	// isActiveがfalseになった瞬間のコールバック関数
	void SetOnDeactivateCallback(const std::function<void()>& callback);

	void SetItem(EnemyAdjustItem* item) { item_ = item; }

private:

	void IsCollision();
	
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

