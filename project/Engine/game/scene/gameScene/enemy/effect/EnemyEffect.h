#pragma once
#include <memory>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"

#include "WorldTransform.h"

class BaseEnemy;

class EnemyEffect
{
public:

	void Init();

	void Update();

	void Draw();

	void SetEnemy(BaseEnemy* enemy) { enemy_ = enemy; }

	// 弾がヒットした時のエフェクトを呼び出す
	void OnceBulletHitEffect(const WorldTransform& transform);
	void OnceBulletHitExplosionEffect(const WorldTransform& transform);
	// 死んだ時に出るエフェクトを出すか
	void SetDeadEffect(bool flag) { deadEmitter_->SetIsCreate(flag); }
	// 敵の弾発射時のエフェクトを出すか
	void SetBulletPredictionEffect(bool flag);

private:

	// レティクルがヒットした時のエフェクト
	void HitReticleInit();
	void HitReticleUpdate();

	// 敵の弾発射時のエフェクト(予測線)
	void BulletPredictionInit();
	void BulletPredictionUpdate();

private:

	BaseEnemy* enemy_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 弾がヒットした時のエフェクト ==================== */

	// 弾がヒットした時のエフェクト(血飛沫)
	std::shared_ptr<ParticleEmitter> hitEmitter_;
	// 弾がヒットした時のエフェクト(爆発)
	std::shared_ptr<ParticleEmitter> hitExplosionEmitter_;
	// 弾がヒットした時のエフェクト(爆発)
	std::shared_ptr<ParticleEmitter> hitRingEmitter_;

	/* ==================== プレイヤーが死亡時のエフェクト ==================== */

	// 死亡のエフェクト
	std::shared_ptr<ParticleEmitter> deadEmitter_ = nullptr;

	/* ==================== レティクルがヒットした時のエフェクト ==================== */

	struct HitReticleEffect {
		std::unique_ptr<PrimitiveDrawr> cylinder_;
		float frame_;
		float axis_;
	};
	HitReticleEffect hitReticleEffect_;

	/* ==================== 敵が弾発射時のエフェクト(予測線) ==================== */

	struct BulletPredictionEffect {
		std::unique_ptr<PrimitiveDrawr> plane_;
	};
	std::vector<BulletPredictionEffect> bulletPredictionEffect_;

};