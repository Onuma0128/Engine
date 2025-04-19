#include "GamePlayScene.h"

#include "ParticleManager.h"
#include "SceneManager.h"
#include "ModelManager.h"

void GamePlayScene::Initialize()
{
	demoObj_ = std::make_unique<Object3d>();
	demoObj_->Initialize("plane.obj");
	demoObj_->SetTexture("resources", "white1x1.png");
	demoObj_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	demoObj_->GetTransform().scale_ = { 20.0f,20.0f ,20.0f };
	demoObj_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, -1.57f);

	player_ = std::make_unique<Player>();
	player_->Init();

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	demoObj_->Update();

	player_->Update();

	gameCamera_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	demoObj_->Draw();

	player_->Draw();

	ParticleManager::GetInstance()->Draw();
}
