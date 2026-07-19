#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	dumbbellSensorController_ = std::make_unique<DumbbellSensorController>();
	dumbbellSensorController_->Initialize();

	numbersUI_ = std::make_unique<NumbersUI>();
    numbersUI_->Initialize();

	dumbbellArmUI_ = std::make_unique<DumbbellArmUI>();
	dumbbellArmUI_->Initialize();

	cheerForUI_ = std::make_unique<CheerForUI>();
	cheerForUI_->Initialize();
}

void GamePlayScene::Finalize()
{
    dumbbellSensorController_->Finalize();
	dumbbellArmUI_->Finalize();
	numbersUI_->Finalize();
	cheerForUI_->Finalize();
}

void GamePlayScene::Update()
{
    dumbbellSensorController_->Update();
	numbersUI_->Update(dumbbellSensorController_->GetDumbbellCount());
	dumbbellArmUI_->Update(dumbbellSensorController_->GetExtendedAngle());
	cheerForUI_->Update(dumbbellSensorController_->GetDumbbellState());
}

void GamePlayScene::Draw()
{
    
}

float GamePlayScene::GetSensorExtendedAngle() const
{
    return dumbbellSensorController_->GetExtendedAngle();
}

float GamePlayScene::GetSensorBentAngle() const
{
    return dumbbellSensorController_->GetBentAngle();
}