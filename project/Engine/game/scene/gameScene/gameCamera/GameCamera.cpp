#include "GameCamera.h"

#include <numbers>

#include "Input.h"
#include "CameraManager.h"
#include "Vector3.h"

#include "gameScene/player/Player.h"

void GameCamera::Init()
{
	GlobalInit();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotation(global_->GetValue<Vector3>("CameraOffset", "rotation"));
	Matrix4x4 rotateMatrix = Matrix4x4::Rotate(camera_->GetRotation());
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation").Transform(rotateMatrix);
	camera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(camera_.get());
	CameraManager::GetInstance()->SetActiveCamera(1);
	camera_->Update();
}

void GameCamera::GlobalInit()
{
	global_->AddValue<Vector3>("CameraOffset", "rotation", Vector3{});
	global_->AddValue<Vector3>("CameraOffset", "translation", Vector3{});
	global_->AddValue<float>("CameraOffset", "minRotateX", 0.2f);
	global_->AddValue<float>("CameraOffset", "maxRotateX", 1.0f);
	global_->AddValue<float>("CameraOffset", "angleSpeed", 0.01f);
	global_->AddValue<float>("CameraOffset", "angleLerp", 0.1f);
}

void GameCamera::Update()
{
	// 回転する速度
	const float angleSpeed = global_->GetValue<float>("CameraOffset", "angleSpeed");
	// 回転した後の角度
	const float rotationX = Input::GetInstance()->GetGamepadRightStickY() * angleSpeed;
	const float rotationY = Input::GetInstance()->GetGamepadRightStickX() * angleSpeed;
	// オフセットの回転角
	const Vector3 offsetRotation = global_->GetValue<Vector3>("CameraOffset", "rotation");
	// カメラのx回転,y回転を制御する
	float min = global_->GetValue<float>("CameraOffset", "minRotateX") - offsetRotation.x;
	float max = global_->GetValue<float>("CameraOffset", "maxRotateX") - offsetRotation.x;
	destinationAngleX += rotationX;
	destinationAngleX = std::clamp(destinationAngleX, min, max);
	destinationAngleY += rotationY;
	// 回転を更新
	rotateAngle_.x = LerpShortAngle(rotateAngle_.x, destinationAngleX, global_->GetValue<float>("CameraOffset", "angleLerp"));
	rotateAngle_.y = LerpShortAngle(rotateAngle_.y, destinationAngleY, global_->GetValue<float>("CameraOffset", "angleLerp"));
	camera_->SetRotation(rotateAngle_ + offsetRotation);

	// カメラの回転に合わせた座標を更新
	Matrix4x4 rotateMatrix = Matrix4x4::Rotate(rotateAngle_);
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation");

	Vector3 previous = camera_->GetTranslation();
	Vector3 current = player_->GetTransform().translation_ + (translation.Transform(rotateMatrix));
	previous = Vector3::Lerp(previous, current, 0.1f);

	camera_->SetTranslation(previous);
}

float GameCamera::LerpShortAngle(float a, float b, float t)
{
	float diff = b - a;

	// 角度を [-2PI, +2PI] に補正する
	while (diff > 2 * std::numbers::pi_v<float>) {
		diff -= 2 * std::numbers::pi_v<float>;
	}
	while (diff < -2 * std::numbers::pi) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 角度を [-PI, +PI] に補正する
	if (diff > std::numbers::pi) {
		diff -= 2 * std::numbers::pi_v<float>;
	} else if (diff < -std::numbers::pi_v<float>) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 線形補間を行う
	return a + t * diff;
}