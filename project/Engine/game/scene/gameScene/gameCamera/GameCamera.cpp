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
	mainCamera_ = std::make_unique<Camera>();
	mainCamera_->Initialize();
	mainCamera_->SetRotation(global_->GetValue<Vector3>("CameraOffset", "rotation"));
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation");
	mainCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(mainCamera_.get());
	CameraManager::GetInstance()->SetActiveCamera(1);
	mainCamera_->Update();

	sabCamera_ = std::make_unique<Camera>();
	sabCamera_->Initialize();
	sabCamera_->SetRotation(global_->GetValue<Vector3>("CameraOffset", "sab_rotation"));
	translation = global_->GetValue<Vector3>("CameraOffset", "sab_translation");
	sabCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(sabCamera_.get());
	sabCamera_->Update();
}

void GameCamera::GlobalInit()
{
	global_->AddValue<Vector3>("CameraOffset", "rotation", Vector3{});
	global_->AddValue<Vector3>("CameraOffset", "translation", Vector3{});

	global_->AddValue<Vector3>("CameraOffset", "sab_rotation", Vector3{});
	global_->AddValue<Vector3>("CameraOffset", "sab_translation", Vector3{});
}

void GameCamera::Update()
{
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_1)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}
	if (input->TriggerKey(DIK_2)) {
		CameraManager::GetInstance()->SetActiveCamera(2);
	}

	// サブカメラの更新
	SabUpdate();

	// オフセットの回転角
	const Vector3 offsetRotation = global_->GetValue<Vector3>("CameraOffset", "rotation");
	// 回転を更新
	mainCamera_->SetRotation(offsetRotation);

	// シェイクオフセット（ランダムな微小ノイズ）
    Vector3 shakeOffset{};
    if (shakeStrength_ > 0.01f) {
        shakeOffset = {
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
        };
        shakeStrength_ *= shakeDecay_; // 減衰させる
    }

	// カメラの回転に合わせた座標を更新
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation");
	Vector3 previous = mainCamera_->GetTranslation();
	Vector3 current = player_->GetTransform().translation_ + translation;

	previous = Vector3::Lerp(previous, current + shakeOffset, 0.1f);
	mainCamera_->SetTranslation(previous);
}

void GameCamera::SabUpdate()
{
	// オフセットの回転角
	const Vector3 offsetRotation = global_->GetValue<Vector3>("CameraOffset", "sab_rotation");
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "sab_translation");
	Vector3 current = player_->GetTransform().translation_ + translation;
	// 回転と座標を更新
	sabCamera_->SetRotation(offsetRotation);
	sabCamera_->SetTranslation(current);
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