#pragma once
#include <functional>

#include "Object3d.h"
#include "Collider.h"

#include "gameScene/player/effect/PlayerBulletEffect.h"

class PlayerAdjustItem;

class PlayerBullet : public Object3d,Collider
{
public:

	void Init(const std::string& colliderName);

	void Update();

	void OnCollisionEnter(Collider* other)override;

	// リロードをする関数
	void Reload(const WorldTransform& transform, bool isEmit = false);
	// 攻撃をする関数
	void Attack(const WorldTransform& transform, float speed = 20.0f);

	bool GetIsActive()const { return isActive_; }
	bool GetIsReload()const { return isReload_; }
	uint32_t GetNockdownCount()const { return kNockdownCount_; }
	void IsCollision();

	// isActiveがfalseになった瞬間のコールバック関数
	void SetOnDeactivateCallback(const std::function<void()>& callback);

	void SetItem(PlayerAdjustItem* item) { item_ = item; }

private:

	// 弾のエフェクト呼び出し
	std::unique_ptr<PlayerBulletEffect> effect_ = nullptr;
	// 調整項目
	PlayerAdjustItem* item_ = nullptr;

	// 今動いているか
	bool isActive_ = false;
	// 前フレームのisActive
	bool wasActive_ = false;
	// コールバック関数
	std::function<void()> onDeactivatedCallback_;

	// 速度
	float speed_ = 0.0f;
	Vector3 velocity_;
	// 動いている時間
	float activeFrame_ = 0.0f;
	// リロードされているか
	bool isReload_ = true;
	// 敵を倒した数を保存する
	uint32_t kNockdownCount_ = 0;

};

