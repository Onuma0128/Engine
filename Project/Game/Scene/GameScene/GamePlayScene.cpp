#include "GamePlayScene.h"

#include "imgui.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "SceneJsonLoader.h"

void GamePlayScene::Initialize()
{
	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	// マップの初期化
	mapCollision_ = std::make_unique<MapCollision>();
	mapCollision_->Init(loader);

	// スカイボックスの初期化
	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::kSkybox);
	skyBox_->GetTransform().scale = { 1024.0f,1024.0f ,1024.0f };
	skyBox_->SetSceneRenderer();

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Init(loader);

	// ゲームカメラの初期化
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

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

	// ゲームシーン全体のUIを初期化
	gameSceneUis_ = std::make_unique<GameSceneUIs>();
	gameSceneUis_->Init();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	if (gameSceneUis_->GetIsSelectIn()) {
		gameSceneUis_->Update();
		return;
	}

	// マップの判定を更新
	mapCollision_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵スポナーと敵の更新
	enemySpawnerFactory_->Update();

	// フィールド上のオブジェクトの更新
	fieldObjectFactory_->Update();

	// ゲームカメラの更新
	gameCamera_->Update();

	// スカイボックスの更新
	skyBox_->Update();

	// ゲームシーン全体のUIの更新
	gameSceneUis_->SetKillCount(player_->GetShot()->GetNockdownCount());
	gameSceneUis_->SetHitRate(player_->GetShot()->GetHitRate());
	gameSceneUis_->Update();

	// パーティクルの更新
	ParticleManager::GetInstance()->Update();

	// プレイヤーが死んだかクリアをしたらセレクトUIを表示する
	uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
	if ((!player_->GetIsAlive() || player_->GetShot()->GetNockdownCount() >= clearKill) && !isSelect_) {
		isSelect_ = true;
		gameSceneUis_->SelectUIFadeIn();
	}
}

void GamePlayScene::Draw()
{
	player_->EffectDraw();

	enemySpawnerFactory_->Draw();

	player_->Draw();

	gameSceneUis_->Draw();
}