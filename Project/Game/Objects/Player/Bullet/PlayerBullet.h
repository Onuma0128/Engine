#pragma once
#include <functional>

#include "Object3d.h"
#include "Collider.h"

#include "objects/player/effect/PlayerBulletEffect.h"

class PlayerAdjustItem;
class PlayerShot;

/// <summary>
/// プレイヤーの弾を表すクラス
/// </summary>
class PlayerBullet : public Object3d,Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="colliderName"></param>
	void Init(const std::string& colliderName);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other)override;

	/// <summary>
	/// リロードをする関数
	/// </summary>
	/// <param name="transform"></プレイヤーのTransformを設定する>
	/// <param name="isEmit"></エフェクトを出すか>
	void Reload(const WorldTransform& transform, bool isEmit = false);

	/// <summary>
	/// 攻撃をする関数
	/// </summary>
	/// <param name="transform"></プレイヤーのTransformを設定する>
	/// <param name="speed"></弾の速度を設定する>
	void Attack(const WorldTransform& transform, float speed = 20.0f);

	/// <summary>
	/// 動いているかを取得する
	/// </summary>
	/// <returns></returns>
	bool GetIsActive()const { return isActive_; }

	/// <summary>
	/// リロード中かを取得する
	/// </summary>
	/// <returns></returns>
	bool GetIsReload()const { return isReload_; }

	/// <summary>
	/// 弾が当たっていれば呼び出す
	/// </summary>
	void IsCollision();

	/// <summary>
	/// isActiveがfalseになった瞬間のコールバック関数
	/// </summary>
	void SetOnDeactivateCallback(const std::function<void()>& callback);

	/// <summary>
	/// 調整項目を設定する
	/// </summary>
	/// <param name="item"></param>
	void SetItem(PlayerAdjustItem* item) { item_ = item; }

	/// <summary>
	/// プレイヤーの弾を撃つクラスのポインタを設定する
	/// </summary>
	/// <param name="playerShot"></param>
	void SetPlayerShot(PlayerShot* playerShot) { playerShot_ = playerShot; }

private:

	// 弾のエフェクト呼び出し
	std::unique_ptr<PlayerBulletEffect> effect_ = nullptr;
	// 調整項目
	PlayerAdjustItem* item_ = nullptr;
	// プレイヤーの弾を撃つクラスのポインタ
	PlayerShot* playerShot_ = nullptr;

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
	float activeTime_ = 0.0f;
	// リロードされているか
	bool isReload_ = true;

};

