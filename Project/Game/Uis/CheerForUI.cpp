#include "CheerForUI.h"

#include "DirectXEngine.h"
#include "imgui.h"

void CheerForUI::Initialize()
{
	cheerForSprite_ = std::make_unique<Sprite>();
	cheerForSprite_->Initialize("cheerForUI.png");
	cheerForSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	cheerForSprite_->GetTransform().size = { 512.0f, 128.0f };
	cheerForSprite_->GetTransform().position = { 681.0f, 650.0f };
	cheerForSprite_->SetTextureSize({ kSourceDigitWidth, kSourceDigitHeight });
	cheerForSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	cheerForSprite_->SetSceneRenderer();
}

void CheerForUI::Finalize()
{
	if (DirectXEngine::GetSceneRenderer() != nullptr)
	{
		if (cheerForSprite_ != nullptr)
		{
			DirectXEngine::GetSceneRenderer()->SetRemoveList(cheerForSprite_.get());
		}
	}
	cheerForSprite_.reset();
}

void CheerForUI::Update(DumbbellPoseCounter::State state)
{
	switch (state)
	{
	case DumbbellPoseCounter::State::WaitingForExtended:
		break;
	case DumbbellPoseCounter::State::WaitingForBent:
		cheerForSprite_->SetTextureLeftTop({ kSourceDigitWidth, kSourceDigitHeight });
		break;
	case DumbbellPoseCounter::State::WaitingForReturn:
		cheerForSprite_->SetTextureLeftTop({ 0.0f, kSourceDigitHeight });
		break;
	default:
		break;
	}

	ImGui::Begin("CheerForUI");
	ImGui::DragFloat2("CheerFor Position", &cheerForSprite_->GetTransform().position.x, 1.0f, 0.0f, 1280.0f);
	ImGui::End();

	cheerForSprite_->Update();
}