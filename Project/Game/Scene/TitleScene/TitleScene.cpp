#include "TitleScene.h"

#include "DirectXEngine.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

void TitleScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotation(Vector3{ 0.55f,0.0f,0.0f });
	camera_->SetTranslation(Vector3{ -0.4f,10.0f,-15.0f });
	CameraManager::GetInstance()->Clear();
	CameraManager::GetInstance()->SetCamera(camera_);
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->Update();

	test = std::make_unique<ParticleEmitter>("test");
	particleManager->CreateParticleGroup(test);
	//test->SetIsCreate(false);

}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	camera_->Update();

	particleManager->Update();
}

void TitleScene::Draw()
{
}
