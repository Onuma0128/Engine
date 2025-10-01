#pragma once

#include <memory>
#include <vector>

#include "Animation.h"
#include "Collider.h"

#include "gameScene/enemy/type/EnemyType.h"
#include "gameScene/enemy/state/EnemyBaseState.h"
#include "gameScene/enemy/bullet/EnemyBullet.h"
#include "gameScene/enemy/effect/EnemyEffect.h"
#include "gameScene/enemy/shadow/EnemyShadow.h"
#include "gameScene/enemy/ray/EnemyRay.h"

#include "gameScene/searchAlgorithm/navigation/PathFinder.h"

// 敵が今どうなっているか
struct EnemyStateParam {
	// 敵を動かして描画するか管理用
	bool enableMove_ = false;
	// 生きているか
	bool isAlive_ = true;
	// 死んだ瞬間
	bool isDead_ = false;
	// プレイヤーのReticleに当たっているか
	bool hitReticle_ = false;
};

class Player;
class GameCamera;
class MapCollision;
class EnemyAdjustItem;

class EnemyWeaponBase;

class BaseEnemy : public Animation, public Collider
{
public:

	/// ==================== 共通メンバ関数 ==================== ///

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();
	// 描画をしていない時の更新
	virtual void TransformUpdate();
	// 描画
	virtual void Draw() = 0;

private:

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:

	// ステートを変える
	void ChengeState(std::unique_ptr<EnemyBaseState> newState);
	// 死亡時呼び出し
	virtual void Dead();
	// リセット時呼び出し
	virtual void Reset(const Vector3& position);

	/// ==================== アクセッサ ==================== ///

	// 敵のタイプ
	EnemyType GetType()const { return type_; }
	// エフェクト
	EnemyEffect* GetEffect() { return effect_.get(); }
	// 弾
	virtual const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
	// ウエポン
	virtual EnemyWeaponBase* GetWeapon() { return nullptr; }
	virtual EnemyWeaponBase* GetShieldWeapon() { return nullptr; }
	// 敵の目線
	EnemyRay* GetEnemyRay() { return ray_.get(); }
	// 当たったプレイヤーの弾座標
	const Vector3& GetPlayerBullet() { return playerBulletPosition_; }
	// 速度
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3& GetVelocity() { return velocity_; }
	// 探索をリセットする
	void ResetSearch();

	// 動けるのが有効か
	bool GetEnableMove()const { return stateParam_.enableMove_; }
	// プレイヤーのレティクルに当たっているか
	bool GetHitReticle()const { return stateParam_.hitReticle_; }
	// 死んでいるか
	bool GetIsDead()const { return stateParam_.isDead_; }
	void SetIsDead(bool flag) { stateParam_.isDead_ = flag; }

	/// ==================== その他ポインタアクセッサ ==================== ///

	// プレイヤー
	Player* GetPlayer() { return player_; }
	void SetPlayer(Player* player) { player_ = player; }
	// カメラ
	GameCamera* GetGameCamera() { return gameCamera_; }
	void SetGameCamera(GameCamera* camera) { gameCamera_ = camera; }
	// 調整項目
	EnemyAdjustItem* GetItem() { return items_; }
	void SetItem(EnemyAdjustItem* item) { items_ = item; }
	// 探索アルゴリズム
	void SetMapData(MapCollision* mapData) { pathFinder_.SetMapData(mapData); }
	PathFinder& GetPathFinder() { return pathFinder_; }

protected:

	// プレイヤー
	Player* player_ = nullptr;
	// カメラ
	GameCamera* gameCamera_ = nullptr;
	// 調整項目
	EnemyAdjustItem* items_ = nullptr;

	// 敵のタイプ
	EnemyType type_;
	// 状態遷移
	std::unique_ptr<EnemyBaseState> state_ = nullptr;
	// 弾
	std::vector<std::unique_ptr<EnemyBullet>> bullets_;
	// エフェクト
	std::unique_ptr<EnemyEffect> effect_ = nullptr;
	// 影
	std::unique_ptr<EnemyShadow> shadow_ = nullptr;
	// 敵の目線
	std::unique_ptr<EnemyRay> ray_ = nullptr;
	// 敵が今どうなっているか
	EnemyStateParam stateParam_;

	// 探索アルゴリズム
	PathFinder pathFinder_;
	// 速度
	Vector3 velocity_{};
	// 当たったプレイヤーの弾の座標を保存
	Vector3 playerBulletPosition_{};
	// OutLineのColor
	Vector3 outlineColor_;
};

