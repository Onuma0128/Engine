#include "TitleScene.h"

#include "DirectXEngine.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

void TitleScene::Initialize()
{
    sceneFade_ = std::make_unique<NumaEngine::BaseUI>();
	sceneFade_->Init("TitleFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(NumaEngine::Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();
	titleUI_->FadeIn();

    // 繧ｷ繝ｼ繝ｳ縺ｮ繝ｭ繝ｼ繝・
	NumaEngine::SceneJsonLoader loader;
	loader.Load("sceneObject");

	// 繝槭ャ繝励・蛻晄悄蛹・
	mapCollision_ = std::make_unique<MapCollision>();
	mapCollision_->Init(loader);

	// 繝励Ξ繧､繝､繝ｼ縺ｮ蛻晄悄蛹・
	player_ = std::make_unique<Player>();
	player_->SetLoader(&loader);
	player_->SetMapData(mapCollision_.get());
	player_->Initialize();
	player_->PlayDemo();

	// 繧ｲ繝ｼ繝繧ｫ繝｡繝ｩ縺ｮ蛻晄悄蛹・
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	// 莉ｲ髢鍋ｮ｡逅・け繝ｩ繧ｹ縺ｮ蛻晄悄蛹・
	companionManager_ = std::make_unique<MuscleCompanionManager>();
	companionManager_->SetPlayer(player_.get());
	companionManager_->SetMapData(mapCollision_.get());
	companionManager_->SetCamera(gameCamera_.get());
	companionManager_->Initialize();
	companionManager_->PlayDemo();
	gameCamera_->SetCompanionManager(companionManager_.get());

	// 繝輔ぅ繝ｼ繝ｫ繝我ｸ翫・繧ｪ繝悶ず繧ｧ繧ｯ繝医・蛻晄悄蛹悶→逕滓・
	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->SetMapCollision(mapCollision_.get());
	fieldObjectFactory_->SetGameCamera(gameCamera_.get());
	fieldObjectFactory_->Init(loader);

	// 謨ｵ繧ｹ繝昴リ繝ｼ縺ｮ蛻晄悄蛹悶→逕滓・
	enemySpawnerFactory_ = std::make_unique<EnemySpawnerFactory>();
	enemySpawnerFactory_->SetPlayer(player_.get());
	enemySpawnerFactory_->SetGameCamera(gameCamera_.get());
	enemySpawnerFactory_->SetMapData(mapCollision_.get());
	enemySpawnerFactory_->Init(loader);
	enemySpawnerFactory_->PlayDemo();
	gameCamera_->SetSpawner(enemySpawnerFactory_.get());

    // BGM繧呈ｵ√☆
	const float kBGMVolume = 0.04f;
	bgm_ = std::make_unique<NumaEngine::Audio>();
	bgm_->SoundPlayWave("GameSceneBGM.wav", kBGMVolume, true);
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	titleUI_->Update();

	sceneFade_->DrawImGui();
	sceneFade_->Update();

	// 繝槭ャ繝励・蛻､螳壹ｒ譖ｴ譁ｰ
	mapCollision_->Update();

	// 繝励Ξ繧､繝､繝ｼ縺ｮ譖ｴ譁ｰ
	player_->Update();

	// 莉ｲ髢鍋ｮ｡逅・け繝ｩ繧ｹ縺ｮ譖ｴ譁ｰ
	companionManager_->Update();

	// 謨ｵ繧ｹ繝昴リ繝ｼ縺ｨ謨ｵ縺ｮ譖ｴ譁ｰ
	enemySpawnerFactory_->Update();

	// 繝輔ぅ繝ｼ繝ｫ繝我ｸ翫・繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ
	fieldObjectFactory_->Update();

	// 繧ｲ繝ｼ繝繧ｫ繝｡繝ｩ縺ｮ譖ｴ譁ｰ
	gameCamera_->Update();

	Input* input = Input::GetInstance();
    if ((input->TriggerGamepadButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeIn();
		titleUI_->FadeOut();
	}
	// 繝輔ぉ繝ｼ繝峨′邨ゅｏ縺｣縺溘ｉ繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺吶ｋ
	if (isFade_ && !sceneFade_->IsPlayAnimation()) {
		NumaEngine::SceneManager::GetInstance()->ChangeScene("Game");
	}

    // 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ譖ｴ譁ｰ
	NumaEngine::ParticleManager::GetInstance()->Update();
}

void TitleScene::Draw()
{
	player_->EffectDraw();

	companionManager_->Draw();

	enemySpawnerFactory_->Draw();

	player_->Draw();

	titleUI_->Draw();

	sceneFade_->Draw();
}


