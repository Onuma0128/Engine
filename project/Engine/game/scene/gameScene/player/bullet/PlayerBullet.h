#pragma once

#include "Object3d.h"
#include "Collider.h"

#include <functional>

class PlayerBullet : public Object3d,Collider
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void OnCollisionEnter(Collider* other)override;

	// リロードをする関数
	void Reload();
	// 攻撃をする関数
	void Attack(const WorldTransform& transform);

	bool GetIsActive()const { return isActive_; }
	bool GetIsReload()const { return isReload_; }
	void IsCollision();
	bool GetIsCollision()const { return isObjectCollision_; }
	void SetIsCollision(bool flag) { isObjectCollision_ = flag; }

	// isActiveがfalseになった瞬間のコールバック関数
	void SetOnDeactivateCallback(const std::function<void()>& callback);

private:

	// 今動いているか
	bool isActive_ = false;
	// 前フレームのisActive
	bool wasActive_ = false;
	// コールバック関数
	std::function<void()> onDeactivatedCallback_;

	// 動いている時間
	float activeFrame_ = 0.0f;
	// リロードされているか
	bool isReload_ = true;
	// 誰かと衝突をしたか
	bool isObjectCollision_ = false;

	Vector3 velocity_;

};

