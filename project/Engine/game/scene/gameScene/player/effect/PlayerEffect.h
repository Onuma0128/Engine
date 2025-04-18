#pragma once

#include "ParticleManager.h"
#include "ParticleEmitter.h"

class PlayerEffect
{
public:

	void Init();

	void Update();

	void Draw();




private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	// プレイヤーが弾を撃った時のエフェクト
	// 弾を撃った時の爆発
	std::unique_ptr<ParticleEmitter> bulletExplosionEmitter_ = nullptr;
	// 弾を撃った時の火花
	std::unique_ptr<ParticleEmitter> bulletSparkEmitter_ = nullptr;
	// 弾を撃った後の煙
	std::unique_ptr<ParticleEmitter> bulletSmokeEmitter_ = nullptr;
};

