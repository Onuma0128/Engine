#pragma once

#include <memory>

#include "Camera.h"
#include "GlobalVariables.h"

class Player;

class GameCamera
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void SetPlayer(Player* player) { player_ = player; }

	float LerpShortAngle(float a, float b, float t);

	Camera* GetCamera() { return camera_.get(); }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Camera> camera_ = nullptr;
	float destinationAngleX = 0.0f;
	float destinationAngleY = 0.0f;
	Vector3 rotateAngle_ = {};

	Player* player_ = nullptr;

};