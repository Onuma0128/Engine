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

	bulletExplosionEmitter_ = std::make_unique<ParticleEmitter>("bulletExplosion");
	ParticleManager::GetInstance()->CreateParticleGroup("bulletExplosion", "circle.png", bulletExplosionEmitter_.get());
	bulletExplosionEmitter_->SetIsCreate(false);

	bulletSparkEmitter_ = std::make_unique<ParticleEmitter>("bulletSpark");
	ParticleManager::GetInstance()->CreateParticleGroup("bulletSpark", "circle.png", bulletSparkEmitter_.get());
	bulletSparkEmitter_->SetIsCreate(false);

	bulletSmokeEmitter_ = std::make_unique<ParticleEmitter>("bulletSmoke");
	ParticleManager::GetInstance()->CreateParticleGroup("bulletSmoke", "smoke.png", bulletSmokeEmitter_.get());
	bulletSmokeEmitter_->SetIsCreate(false);
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	testObj_->Update();

	animation_->Update();
	animation2_->Update();

	// ボタンを押したらパーティクルを発生させる
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE)) {
		Vector3 position = { 0.0f,3.0f,0.0f };
		bulletExplosionEmitter_->SetPosition(position);
		bulletSparkEmitter_->SetPosition(position);
		bulletSmokeEmitter_->SetPosition(position + Vector3{ 0.0f,0.2f,0.0f });

		bulletExplosionEmitter_->onceEmit();
		bulletSparkEmitter_->onceEmit();
		bulletSmokeEmitter_->onceEmit();
	}

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{	
	testObj_->Draw();

	animation_->Draw();
	animation2_->Draw();

	ParticleManager::GetInstance()->Draw();
}
