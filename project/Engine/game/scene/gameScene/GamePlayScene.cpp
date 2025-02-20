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
	animation_->Init();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	testObj_->Update();

	animation_->Update();
}

void GamePlayScene::Draw()
{	
	testObj_->Draw();

	animation_->Draw();
}
