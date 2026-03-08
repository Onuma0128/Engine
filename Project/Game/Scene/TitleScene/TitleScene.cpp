#include "TitleScene.h"

#include "DirectXEngine.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

void TitleScene::Initialize()
{
	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("TitleFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();
	titleUI_->FadeIn();

	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	// マップの初期化
	mapCollision_ = std::make_unique<MapCollision>();
	mapCollision_->Init(loader);

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->SetLoader(&loader);
	player_->SetMapData(mapCollision_.get());
	player_->Initialize();
	player_->PlayDemo();

	// ゲームカメラの初期化
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	// 仲間管理クラスの初期化
	companionManager_ = std::make_unique<MuscleCompanionManager>();
	companionManager_->SetPlayer(player_.get());
	companionManager_->SetMapData(mapCollision_.get());
	companionManager_->SetCamera(gameCamera_.get());
	companionManager_->Initialize();
	companionManager_->PlayDemo();
	gameCamera_->SetCompanionManager(companionManager_.get());

	// フィールド上のオブジェクトの初期化と生成
	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->SetMapCollision(mapCollision_.get());
	fieldObjectFactory_->SetGameCamera(gameCamera_.get());
	fieldObjectFactory_->Init(loader);

	// 敵スポナーの初期化と生成
	enemySpawnerFactory_ = std::make_unique<EnemySpawnerFactory>();
	enemySpawnerFactory_->SetPlayer(player_.get());
	enemySpawnerFactory_->SetGameCamera(gameCamera_.get());
	enemySpawnerFactory_->SetMapData(mapCollision_.get());
	enemySpawnerFactory_->Init(loader);
	enemySpawnerFactory_->PlayDemo();
	gameCamera_->SetSpawner(enemySpawnerFactory_.get());

	// BGMを流す
	const float kBGMVolume = 0.04f;
	bgm_ = std::make_unique<Audio>();
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

	// マップの判定を更新
	mapCollision_->Update();

	// プレイヤーの更新
	player_->Update();

	// 仲間管理クラスの更新
	companionManager_->Update();

	// 敵スポナーと敵の更新
	enemySpawnerFactory_->Update();

	// フィールド上のオブジェクトの更新
	fieldObjectFactory_->Update();

	// ゲームカメラの更新
	gameCamera_->Update();

	Input* input = Input::GetInstance();
    if ((input->TriggerGamepadButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeIn();
		titleUI_->FadeOut();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->IsPlayAnimation()) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	// パーティクルの更新
	ParticleManager::GetInstance()->Update();
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
