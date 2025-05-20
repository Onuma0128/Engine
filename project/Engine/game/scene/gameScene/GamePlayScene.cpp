#include "GamePlayScene.h"

#include "ParticleManager.h"
#include "SceneManager.h"

#include "gameScene/enemy/Enemy.h"

#include "Collision3D.h"
#include "Vector3.h"

void GamePlayScene::Initialize()
{
	demoObj_ = std::make_unique<Object3d>();
	demoObj_->Initialize("terrain.obj");
	//demoObj_->SetTexture("resources", "uvChecker.png");
	//demoObj_->SetColor({ 0.1f,0.1f,0.1f,1.0f });
	//demoObj_->GetTransform().scale_ = { 20.0f,20.0f,20.0f };
	//demoObj_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, -1.57f);

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 512.0f,512.0f ,512.0f };
	skyBox_->GetRenderOptions().enabled = false;

	player_ = std::make_unique<Player>();
	player_->Init();

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	enemySpawner_ = std::make_unique<EnemySpawner>();
	enemySpawner_->SetPlayer(player_.get());
	enemySpawner_->SetGameCamera(gameCamera_.get());
	enemySpawner_->Init();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	demoObj_->Update();

	player_->Update();

	enemySpawner_->Update();

	gameCamera_->Update();

	skyBox_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
}