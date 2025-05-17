#pragma once

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

class Enemy;

class EnemyEffect
{
public:

	void Init();

	void Update();

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	// 弾がヒットした時のエフェクトを呼び出す
	void OnceBulletHitEffect(const WorldTransform& transform);

private:

	Enemy* enemy_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 敵に弾がヒットした時のエフェクト ==================== */

	// 弾がヒットした時のエフェクト(血飛沫)
	std::unique_ptr<ParticleEmitter> hitEmitter_;
	// 弾がヒットした時のエフェクト(土煙)
	std::unique_ptr<ParticleEmitter> hitDustEmitter_;

};