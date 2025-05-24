#pragma once
#include <memory>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"

#include "WorldTransform.h"

class Enemy;

class EnemyEffect
{
public:

	void Init();

	void Update();

	void Draw();

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	// 弾がヒットした時のエフェクトを呼び出す
	void OnceBulletHitEffect(const WorldTransform& transform);
	void OnceBulletHitExplosionEffect(const WorldTransform& transform);
	// 死んだ時に出るエフェクトを出すか
	void SetDeadEffect(bool flag) { deadEmitter_->SetIsCreate(flag); }

private:

	void HitReticleUpdate();

private:

	Enemy* enemy_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 弾がヒットした時のエフェクト ==================== */

	// 弾がヒットした時のエフェクト(血飛沫)
	std::shared_ptr<ParticleEmitter> hitEmitter_;
	// 弾がヒットした時のエフェクト(土煙)
	std::shared_ptr<ParticleEmitter> hitExplosionEmitter_;

	/* ==================== プレイヤーが死亡時のエフェクト ==================== */

	// 死亡のエフェクト
	std::shared_ptr<ParticleEmitter> deadEmitter_ = nullptr;


	// レティクルがヒットした時のエフェクト
	struct HitReticleEffect {
		std::unique_ptr<PrimitiveDrawr> cylinder_;
		float frame_;
		float axis_;
	};
	HitReticleEffect hitReticleEffect_;


};