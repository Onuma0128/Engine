#include "TitleScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "imgui.h"

void TitleScene::Initialize()
{
	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("TitleFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

	backGround_ = std::make_unique<BaseUI>();
	backGround_->Init("TitleBackGround", "GameData");

    doorOpenL_ = std::make_unique<BaseUI>();
    doorOpenL_->Init("doorOpenL", "GameData");

    doorOpenR_ = std::make_unique<BaseUI>();
    doorOpenR_->Init("doorOpenR", "GameData");
	doorOpenR_->GetSprite()->SetFlipX(true);

	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize("titleUI.png");
	titleSprite_->GetTransform().position = { 640.0f, 200.0f };
	titleSprite_->GetTransform().size = { 768.0f, 128.0f };
	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	titleBaseSize_ = titleSprite_->GetTransform().size;
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_SPACE) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeIn();
        doorOpenL_->FadeIn();
		doorOpenR_->FadeIn();
        isDoorOpen_ = true;
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->IsPlayAnimation()) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
    float alpha = titleSprite_->GetColor().w;
    if (isFade_ && alpha > 0.0f) {
        titleSprite_->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, alpha - 0.1f });
    }

    UpdateTitleHeartbeat();

	titleSprite_->Update();
    doorOpenL_->Update();
    doorOpenR_->Update();
	backGround_->Update();
	sceneFade_->Update();
}

void TitleScene::Draw()
{
	backGround_->Draw();
    doorOpenL_->Draw();
    doorOpenR_->Draw();
	titleSprite_->Draw();
	sceneFade_->Draw();
}

void TitleScene::UpdateTitleHeartbeat()
{
    constexpr float deltaTime = 1.0f / 60.0f;

    heartbeatTimer_ += deltaTime;

    if (heartbeatTimer_ >= heartbeatDuration_)
    {
        heartbeatTimer_ -= heartbeatDuration_;
    }

    // 0.0～1.0の周期位置
    const float phase = heartbeatTimer_ / heartbeatDuration_;

    float pulse = 0.0f;

    // 1回目の大きな鼓動
    if (phase < 0.12f)
    {
        const float localPhase = phase / 0.12f;
        pulse = std::sin(localPhase * 3.14159265f);
    }
    // 2回目の少し小さな鼓動
    else if (phase >= 0.18f && phase < 0.30f)
    {
        const float localPhase = (phase - 0.18f) / 0.12f;
        pulse = std::sin(localPhase * 3.14159265f) * 0.65f;
    }

    const float scale = 1.0f + pulse * heartbeatScale_;

    titleSprite_->GetTransform().size =
    {
        titleBaseSize_.x * scale,
        titleBaseSize_.y * scale
    };
}