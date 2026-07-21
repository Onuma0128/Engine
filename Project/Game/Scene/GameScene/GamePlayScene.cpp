#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	dumbbellSensorController_ = std::make_unique<DumbbellSensorController>();
	dumbbellSensorController_->Initialize();

	numbersUI_ = std::make_unique<NumbersUI>();
    numbersUI_->Initialize();

	dumbbellArm_ = std::make_unique<DumbbellArmUI>();
	dumbbellArm_->Initialize();

	cheerForUI_ = std::make_unique<CheerForUI>();
	cheerForUI_->Initialize();

	armSettingUI_ = std::make_unique<BaseUI>();
	armSettingUI_->Init("ArmSettingUI", "GameData");
	//armSettingUI_->FadeOut();
}

void GamePlayScene::Finalize()
{
    dumbbellSensorController_->Finalize();
	dumbbellArm_->Finalize();
	numbersUI_->Finalize();
	cheerForUI_->Finalize();
}

void GamePlayScene::Update()
{
    dumbbellSensorController_->Update();
	numbersUI_->Update(dumbbellSensorController_->GetDumbbellCount());
	dumbbellArm_->Update(dumbbellSensorController_->GetExtendedAngle());
	cheerForUI_->Update(dumbbellSensorController_->GetDumbbellState());

	armSettingUI_->DrawImGui();
	armSettingUI_->Update();
}

void GamePlayScene::Draw()
{
	armSettingUI_->Draw();
}

float GamePlayScene::GetSensorExtendedAngle() const
{
    return dumbbellSensorController_->GetExtendedAngle();
}

float GamePlayScene::GetSensorBentAngle() const
{
    return dumbbellSensorController_->GetBentAngle();
}