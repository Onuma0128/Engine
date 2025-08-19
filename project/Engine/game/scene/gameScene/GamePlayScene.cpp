#include "GamePlayScene.h"

#include "imgui.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

#include "Collision3D.h"
#include "Vector3.h"

void GamePlayScene::Initialize()
{
	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	mapCollision_ = std::make_unique<MapCollision>();
	mapCollision_->Init(loader);

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 1024.0f,1024.0f ,1024.0f };
	skyBox_->SetSceneRenderer();

	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->Init(loader);

	player_ = std::make_unique<Player>();
	player_->Init(loader);

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	enemySpawnerFactory_ = std::make_unique<EnemySpawnerFactory>();
	enemySpawnerFactory_->SetPlayer(player_.get());
	enemySpawnerFactory_->SetGameCamera(gameCamera_.get());
	enemySpawnerFactory_->SetMapData(mapCollision_.get());
	enemySpawnerFactory_->Init(loader);

	sceneFade_ = std::make_unique<SceneFade>();
	sceneFade_->Init();
	sceneFade_->FadeIn(3.0f);
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	mapCollision_->Update();

	player_->Update();

	enemySpawnerFactory_->Update();

	fieldObjectFactory_->Update();

	gameCamera_->Update();

	skyBox_->Update();

	ParticleManager::GetInstance()->Update();

	sceneFade_->Update();

	// プレイヤーが死んだかクリアをしたらフェードをする
	uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
	if ((!player_->GetIsAlive() || player_->GetShot()->GetNockdownCount() >= clearKill) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeOut();
	}
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isFade_ = true;
		sceneFade_->FadeOut();
	}

	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->GetIsFade()) {
		if (!player_->GetIsAlive()) {
			SceneManager::GetInstance()->ChangeScene("Over");
		} else {
			SceneManager::GetInstance()->ChangeScene("Clear");
		}
	}
}

void GamePlayScene::Draw()
{
	player_->EffectDraw();

	enemySpawnerFactory_->Draw();

	player_->Draw();

	sceneFade_->Draw();
}