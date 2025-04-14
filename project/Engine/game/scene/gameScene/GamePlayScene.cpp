#include "GamePlayScene.h"

#include "ParticleManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "imgui.h"
#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	testObj_ = std::make_unique<TestObject>();
	testObj_->Init();

	animation_ = std::make_unique<Animation>();
	animation_->Init("resources/human", "walk.gltf");
	animation_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, 3.14f);
	animation_->GetTransform().translation_ = { -1.0f,0,0 };

	animation2_ = std::make_unique<Animation>();
	animation2_->Init("resources/human", "sneakWalk.gltf");
	animation2_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, 3.14f);
	animation2_->GetTransform().translation_ = { 1.0f,0,0 };

	testEmitter_ = std::make_unique<ParticleEmitter>("test");
	ParticleManager::GetInstance()->CreateParticleGroup("test", "circle.png", testEmitter_.get());
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	testObj_->Update();

	animation_->Update();
	animation2_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{	
	testObj_->Draw();

	animation_->Draw();
	animation2_->Draw();

	ParticleManager::GetInstance()->Draw();
}
