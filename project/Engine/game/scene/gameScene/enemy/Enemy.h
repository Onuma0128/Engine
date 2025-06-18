#pragma once
#include <vector>
#include <memory>

#include "Object3d.h"
#include "Collider.h"

#include "gameScene/enemy/state/EnemyBaseState.h"
#include "gameScene/enemy/effect/EnemyEffect.h"
#include "gameScene/enemy/bullet/EnemyBullet.h"
#include "gameScene/enemy/type/EnemyType.h"

class Player;
class GameCamera;
class EnemyAdjustItem;

class Enemy:public Object3d,Collider
{
public:

	void Finalize();

	void Init();

	void Update();

	void Draw();

	void ChengeState(std::unique_ptr<EnemyBaseState> newState);

	void Reset(const Vector3& position);

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	/* ============================== ポインタ ============================== */

	EnemyEffect* GetEffect() { return effect_.get(); }

	std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player; }

	GameCamera* GetGameCamera() { return gameCamera_; }
	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }

	EnemyAdjustItem* GetItem() { return items_; }
	void SetItem(EnemyAdjustItem* item) { items_ = item; }

	/* ============================== アクセッサ ============================== */

	EnemyType GetType()const { return type_; }
	
	const Vector3& GetPlayerBullet() { return playerBulletPosition_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3& GetVelocity() { return velocity_; }

	bool GetHitReticle()const { return hitReticle_; }

	bool GetIsDead()const { return isDead_; }
	void SetIsDead(bool flag) { isDead_ = flag; }

private:

	void BulletInit();

private:

	// ゲッターで貰ったポインタ
	Player* player_ = nullptr;
	GameCamera* gameCamera_ = nullptr;
	EnemyAdjustItem* items_ = nullptr;

	// 敵のタイプ
	EnemyType type_ = EnemyType::Ranged;
	// 状態遷移
	std::unique_ptr<EnemyBaseState> state_ = nullptr;
	// エフェクト
	std::unique_ptr<EnemyEffect> effect_ = nullptr;
	// 弾
	std::vector<std::unique_ptr<EnemyBullet>> bullets_;

	// 速度
	Vector3 velocity_{};
	// 生きているか
	bool isAlive_ = true;
	// 死んだ瞬間
	bool isDead_ = false;
	// プレイヤーのReticleに当たっているか
	bool hitReticle_ = false;

	// 当たったプレイヤーの弾の座標を保存
	Vector3 playerBulletPosition_{};

};

