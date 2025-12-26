#pragma once

#include <memory>
#include <vector>

#include "Objects/Character/BaseCharacter.h"

#include "Objects/Enemy/Type/EnemyType.h"
#include "Objects/Enemy/State/EnemyBaseState.h"
#include "Objects/Enemy/Bullet/EnemyBullet.h"
#include "Objects/Enemy/Effect/EnemyEffect.h"
#include "Objects/Enemy/Ray/EnemyRay.h"
#include "SearchAlgorithm/Navigation/PathFinder.h"

// 敵が今どうなっているか
struct EnemyStateParam {
	// 敵を動かして描画するか管理用
	bool enableMove_ = false;
	// 攻撃中か
	bool isAttacking_ = false;
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

/// <summary>
/// 敵の基底クラス
/// </summary>
class BaseEnemy : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画をしていない時の更新
	/// </summary>
	virtual void TransformUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override = 0;

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:

	/// <summary>
	/// ステートを変える
	/// </summary>
	/// <param name="newState"></新しいステートを入れる>
	void ChangeState(std::unique_ptr<EnemyBaseState> newState);

	/// <summary>
	/// 死亡時呼び出し
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// リセット時呼び出し
	/// </summary>
	/// <param name="position"></スポーンする座標を入れる>
	virtual void Reset(const Vector3& position);

	/// <summary>
	/// 経路探索をリセットする
	/// </summary>
	void ResetSearch();

	/// <summary>
	/// 敵のタイプごとの攻撃ステートに遷移する
	/// </summary>
	void TypeChengeAttackState();

	/// <summary>
	/// 敵のタイプごとの攻撃ステートへの距離を取得する
	/// </summary>
	/// <returns></returns>
	const float GetTypeAttackDistance();

	/// ==================== アクセッサ ==================== ///

	// 敵のタイプ
	EnemyType GetType()const { return type_; }
	// 当たったプレイヤーの弾座標
	const Vector3& GetPlayerBullet() { return playerBulletPosition_; }
	// 速度
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3& GetVelocity() { return velocity_; }

	// 動けるのが有効か
	bool GetEnableMove()const { return stateParam_.enableMove_; }
	// プレイヤーのレティクルに当たっているか
	bool GetHitReticle()const { return stateParam_.hitReticle_; }
	// 死んでいるか
	bool GetIsAttacking()const { return stateParam_.isAttacking_; }
	void SetIsAttacking(bool flag) { stateParam_.isAttacking_ = flag; }
	// 死んでいるか
	bool GetIsDead()const { return stateParam_.isDead_; }
	void SetIsDead(bool flag) { stateParam_.isDead_ = flag; }

	/// ==================== その他ポインタアクセッサ ==================== ///

	// エフェクト
	EnemyEffect* GetEffect() { return effect_.get(); }
	// 弾
	virtual const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
	// ウエポン
	virtual EnemyWeaponBase* GetWeapon() { return nullptr; }
	virtual EnemyWeaponBase* GetShieldWeapon() { return nullptr; }
	// 敵の目線
	EnemyRay* GetEnemyRay() { return ray_.get(); }
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
	// 当たったCollider
	const Collider* GetHitCollider()const { return hitCollider_; }

protected:

	// プレイヤー
	Player* player_ = nullptr;
	// カメラ
	GameCamera* gameCamera_ = nullptr;
	// 調整項目
	EnemyAdjustItem* items_ = nullptr;

	// 敵のMaxHP
	uint32_t maxHp_ = 1;
	// 敵の現在のHP
	uint32_t currentHp_ = 1;
	// 敵のタイプ
	EnemyType type_;
	// 状態遷移
	std::unique_ptr<EnemyBaseState> state_ = nullptr;
	// 弾
	std::vector<std::unique_ptr<EnemyBullet>> bullets_;
	// エフェクト
	std::unique_ptr<EnemyEffect> effect_ = nullptr;
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
	// 当たってきたColliderを保存
	Collider* hitCollider_ = nullptr;

};

