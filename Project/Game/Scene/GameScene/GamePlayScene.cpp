#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	dumbbellSensorController_.Initialize();
}

void GamePlayScene::Finalize()
{
    dumbbellSensorController_.Finalize();
}

void GamePlayScene::Update()
{
    dumbbellSensorController_.Update();
}

void GamePlayScene::Draw()
{
    
}

float GamePlayScene::GetSensorExtendedAngle() const
{
    return dumbbellSensorController_.GetExtendedAngle();
}

float GamePlayScene::GetSensorBentAngle() const
{
    return dumbbellSensorController_.GetBentAngle();
}