#pragma once

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

class FieldObjectEffect
{
public:

	void Init();

	void Update();

	// 弾があった時のエフェクトを呼び出す
	void OnceWoodChipEffect(const WorldTransform& transform);

private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== プレイヤーが移動時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> woodChipEmitter_ = nullptr;
};

