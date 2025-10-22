#pragma once

#include <memory>

#include "JsonFunction.h"
#include "Camera.h"
#include "Vector3.h"

class Player;

/// <summary>
/// ゲーム上のカメラを持つ
/// </summary>
class GameCamera
{
public:

	void Init();

	void JsonInit();
	void SaveJson();
	void ValueImGui();

	void Update();
	void SabUpdate(const Vector3& shakeOffset);

	void SetShake(float shakeStrength) { shakeStrength_ = shakeStrength; }

	void SetPlayer(Player* player) { player_ = player; }

	float LerpShortAngle(float a, float b, float t);

	Camera* GetCamera() { return mainCamera_.get(); }

private:

	Player* player_ = nullptr;
	bool playerIsAlive_ = true;

	std::unique_ptr<Camera> mainCamera_ = nullptr;
	std::unique_ptr<Camera> sabCamera_ = nullptr;

	Vector3 mainCameraAddPos_ = {};

	float shakeStrength_ = 0.0f;
	float shakeDecay_ = 0.9f;

	struct SabCameraAnimation {
		Vector3 sabCameraOffset = {};
		bool isRotate = false;
		float rotateTimer = 0.0f;
		float rotateSpeed = 1.0f;
		float radius = 10.0f;
		float positionY;
	};
	SabCameraAnimation sabAnima_;

	JsonFunction data_;

};