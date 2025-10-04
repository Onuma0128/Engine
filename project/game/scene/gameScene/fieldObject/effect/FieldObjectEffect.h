#pragma once
#include <string>

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

class FieldObjectEffect
{
public:

	void Init(const std::string& name);

	void Update();

	// 弾がヒット時のエフェクトを呼び出す
	void OnceWoodChipEffect(const WorldTransform& transform);

private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 弾がヒット時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> hitEmitter_ = nullptr;
};

