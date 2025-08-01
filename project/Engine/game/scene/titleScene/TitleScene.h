#pragma once
#include <memory>

#include "BaseScene.h"
#include "Object3d.h"
#include "Animation.h"
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

	std::unique_ptr<SceneFade> sceneFade_ = nullptr;
	bool isFade_ = false;

	std::unique_ptr<TitleUI> titleUI_ = nullptr;

	// スカイボックス
	std::unique_ptr<PrimitiveDrawr> skyBox_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager = ParticleManager::GetInstance();
	// エフェクト
	std::shared_ptr<ParticleEmitter> test = nullptr;
	std::vector<std::unique_ptr<Animation>> testAnimas_;
	std::unique_ptr<Object3d> box_ = nullptr;
	float t = 0.0f;
};