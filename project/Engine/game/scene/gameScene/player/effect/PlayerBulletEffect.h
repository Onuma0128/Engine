#pragma once

#include <array>

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

class PlayerBulletEffect
{
public:

	void Init();

	void Update();

	// 弾のトレイルエフェクトを呼び出す
	void OnceBulletTrailEffect(const WorldTransform& transform);
	// 弾が消える時のエフェクトを呼び出す
	void OnceBulletDeleteEffect(const WorldTransform& transform);
	// 弾のエフェクトを呼び出す
	void OnceBulletEffect(const WorldTransform& transform);

private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

	// 弾のトレイルエフェクト
	std::shared_ptr<ParticleEmitter> bulletTrailEmitter_;
	// 弾が消える時のエフェクト
	std::shared_ptr<ParticleEmitter> bulletDeleteEmitter_;

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

	// 弾を撃った時の爆発
	std::shared_ptr<ParticleEmitter> bulletExplosionEmitter_ = nullptr;
	// 弾を撃った時の火花
	std::shared_ptr<ParticleEmitter> bulletSparkEmitter_ = nullptr;
	// 弾を撃った後の煙
	std::shared_ptr<ParticleEmitter> bulletSmokeEmitter_ = nullptr;
	// 弾を撃った後の薬莢
	std::shared_ptr<ParticleEmitter> bulletCartridgeEmitter_ = nullptr;
};

