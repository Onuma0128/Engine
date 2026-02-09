#pragma once
#include <memory>

#include "BaseScene.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Animation.h"
#include "Camera.h"
#include "Audio.h"

#include "Scene/SceneFade/SceneFade.h"
#include "Uis/TitleUI/TitleUI.h"
#include "Objects/FieldObject/Factory/FieldObjectFactory.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	std::shared_ptr<Camera> camera_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager = ParticleManager::GetInstance();

	// エフェクト
	std::shared_ptr<ParticleEmitter> test = nullptr;

};