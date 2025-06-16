#include "GamePlayScene.h"

#include "imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "SceneJsonLoader.h"

#include "gameScene/enemy/Enemy.h"

#include "Collision3D.h"
#include "Vector3.h"

void GamePlayScene::Initialize()
{
	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 512.0f,512.0f ,512.0f };
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
	enemySpawnerFactory_->Init(loader);

	emitter_ = std::make_unique<ParticleEmitter>("test");
	ParticleManager::GetInstance()->CreateParticleGroup(emitter_);

}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	player_->Update();

	enemySpawnerFactory_->Update();

	fieldObjectFactory_->Update();

	gameCamera_->Update();

	skyBox_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	enemySpawnerFactory_->Draw();

	player_->Draw();
}