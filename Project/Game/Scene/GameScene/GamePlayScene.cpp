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
	skyBox_->GetTransform().scale = Vector3::ExprUnitXYZ * 1024.0f;
	skyBox_->SetSceneRenderer();

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->SetLoader(&loader);
	player_->Initialize();

	// 仲間管理クラスの初期化
	companionManager_ = std::make_unique<MuscleCompanionManager>();
	companionManager_->SetPlayer(player_.get());
	companionManager_->SetMapData(mapCollision_.get());
	companionManager_->Initialize();

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

	// ボス敵の初期化と生成
	bossEnemy_ = std::make_unique<BossEnemy>();
	bossEnemy_->SetMapData(mapCollision_.get());
	bossEnemy_->SetPlayer(player_.get());
	bossEnemy_->SetEnemySpawnerFactory(enemySpawnerFactory_.get());
	bossEnemy_->Initialize();

	// ゲームシーン全体のUIを初期化
	gameSceneUis_ = std::make_unique<GameSceneUIs>();
	gameSceneUis_->Init();

	// BGMを流す
	const float kBGMVolume = 0.04f;
	bgm_ = std::make_unique<Audio>();
	bgm_->SoundPlayWave("GameSceneBGM.wav", kBGMVolume, true);
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

	// 仲間管理クラスの更新
	companionManager_->Update();

	// 敵スポナーと敵の更新
	enemySpawnerFactory_->Update();

	// ボスをスタートさせる
	uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
	if (bossEnemy_->GetBossState() == BossState::Idle && enemySpawnerFactory_->GetKnockdownCount() >= clearKill) {
		bossEnemy_->StartBossEnemy();
	}
	// ボス敵の更新
	bossEnemy_->Update();

	// フィールド上のオブジェクトの更新
	fieldObjectFactory_->Update();

	// ゲームカメラの更新
	gameCamera_->Update();

	// スカイボックスの更新
	skyBox_->Update();

	// ゲームシーン全体のUIの更新
	gameSceneUis_->SetKillCount(enemySpawnerFactory_->GetKnockdownCount());
	gameSceneUis_->Update();

	// パーティクルの更新
	ParticleManager::GetInstance()->Update();

	// プレイヤーが死んだかクリアをしたらセレクトUIを表示する
	if ((!player_->GetIsAlive() || bossEnemy_->GetBossState() == BossState::Dead) && !isSelect_) {
		isSelect_ = true;
		gameSceneUis_->SelectUIFadeIn();
	}
}

void GamePlayScene::Draw()
{
	player_->EffectDraw();

	bossEnemy_->Draw();

	companionManager_->Draw();

	enemySpawnerFactory_->Draw();

	player_->Draw();

	gameSceneUis_->Draw();
}