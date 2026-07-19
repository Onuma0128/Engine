#include "TitleScene.h"

#include "Input.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("TitleFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	sceneFade_->Update();

	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeIn();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->IsPlayAnimation()) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void TitleScene::Draw()
{
	sceneFade_->Draw();
}
