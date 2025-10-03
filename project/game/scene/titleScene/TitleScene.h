#pragma once
#include <memory>

#include "BaseScene.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"
#include "Camera.h"

#include "scene/sceneFade/SceneFade.h"
#include "scene/titleScene/titleUI/TitleUI.h"

class TitleScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<PrimitiveDrawr> plane_ = nullptr;

	std::unique_ptr<SceneFade> sceneFade_ = nullptr;
	bool isFade_ = false;

	std::unique_ptr<TitleUI> titleUI_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager = ParticleManager::GetInstance();
	// エフェクト
	std::shared_ptr<ParticleEmitter> test = nullptr;

	// エフェクト
	std::shared_ptr<ParticleEmitter> enemyMeleeAttack_ = nullptr;
	// エフェクト
	std::shared_ptr<ParticleEmitter> playerHit_ = nullptr;


};