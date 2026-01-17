#pragma once
#include <memory>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"
#include "WorldTransform.h"

#include "EnemyBulletPredictionEffect.h"

// 前方宣言
class BaseEnemy;

/// <summary>
/// 敵のエフェクトの管理
/// </summary>
class EnemyEffect
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 敵のポインタを取得する
	/// </summary>
	/// <param name="enemy"></param>
	void SetEnemy(BaseEnemy* enemy) { enemy_ = enemy; }

	/// <summary>
	/// 移動時のエフェクトを呼び出す
	/// </summary>
	void OnceMoveEffect(const WorldTransform& transform);

	/// <summary>
	/// 弾を撃つ時のエフェクトを呼び出す
	/// </summary>
	/// <param name="transform"></敵のTransformを入れる>
	void OnceBulletEffect(const WorldTransform& transform);
	
	/// <summary>
	/// 弾がヒットした時のエフェクトを呼び出す
	/// </summary>
	/// <param name="transform"></敵のTransformを入れる>
	void OnceBulletHitEffect(const WorldTransform& transform);

	/// <summary>
	/// 弾がヒットした時の爆発エフェクトを呼び出す
	/// </summary>
	/// <param name="transform"></敵のTransformを入れる>
	void OnceBulletHitExplosionEffect(const WorldTransform& transform);
	void OnceBulletHitExplosionBlueEffect(const WorldTransform& transform);

	/// <summary>
	/// 近接敵攻撃時のエフェクトをセットする
	/// </summary>
	/// <param name="transform"></敵のTransformを入れる>
	void SetMeleeAttackEffect(const WorldTransform& transform);

	/// <summary>
	/// 近接敵攻撃時のエフェクトを出すか
	/// </summary>
	/// <param name="flag"><trueなら出す、falseなら出さない>
	void CreateMeleeAttackEffect(bool flag) { enemyMeleeAttack_->SetIsCreate(flag); }
	
	/// <summary>
	/// 死んだ時に出るエフェクトを出すか
	/// </summary>
	/// <param name="flag"></param>
	void SetDeadEffect(bool flag) { deadEmitter_->SetIsCreate(flag); }

	/// <summary>
	/// 敵の弾発射時のエフェクトを出すか
	/// </summary>
	/// <param name="flag"></param>
	void SetBulletPredictionEffect(bool flag);


private:

	/// <summary>
	/// レティクルがヒットした時のエフェクトの初期化
	/// </summary>
	void HitReticleInit();
	/// <summary>
	/// レティクルがヒットした時のエフェクトの更新
	/// </summary>
	void HitReticleUpdate();

	/// <summary>
	/// 敵の弾発射時のエフェクトの初期化
	/// </summary>
	void BulletPredictionInit();
	/// <summary>
	/// 敵の弾発射時のエフェクトの更新
	/// </summary>
	void BulletPredictionUpdate();

private:

	BaseEnemy* enemy_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 敵が移動時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> moveDustEmitter_ = nullptr;

	/* ==================== 仲間がヒットした時のエフェクト ==================== */

	// 仲間がヒットした時のエフェクト(血飛沫)
	std::shared_ptr<ParticleEmitter> hitEmitter_;
	// 仲間がヒットした時のエフェクト(爆発)
	std::shared_ptr<ParticleEmitter> hitExplosionEmitter_;
	std::shared_ptr<ParticleEmitter> hitExplosionBlueEmitter_;
	// 仲間がヒットした時のエフェクト(爆発)
	std::shared_ptr<ParticleEmitter> hitRingEmitter_;
	std::shared_ptr<ParticleEmitter> hitRingBlueEmitter_;

	/* ==================== 敵が死亡時のエフェクト ==================== */

	// 死亡のエフェクト
	std::shared_ptr<ParticleEmitter> deadEmitter_ = nullptr;

	/* ==================== 敵の攻撃時のエフェクト ==================== */

	std::shared_ptr<ParticleEmitter> enemyMeleeAttack_ = nullptr;

	/* ==================== レティクルがヒットした時のエフェクト ==================== */

	struct HitReticleEffect {
		std::unique_ptr<PrimitiveDrawr> cylinder_;
		float frame_;
		float axis_;
	};
	HitReticleEffect hitReticleEffect_;

	/* ==================== 敵が弾発射時のエフェクト(予測線) ==================== */

	std::vector<std::unique_ptr<EnemyBulletPredictionEffect>> bulletPredictionEffect_;

	// 弾を撃った時の爆発
	std::shared_ptr<ParticleEmitter> bulletExplosionEmitter_ = nullptr;
	// 弾を撃った時の火花
	std::shared_ptr<ParticleEmitter> bulletSparkEmitter_ = nullptr;
	// 弾を撃った後の煙
	std::shared_ptr<ParticleEmitter> bulletSmokeEmitter_ = nullptr;

};