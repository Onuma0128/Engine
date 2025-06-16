#include "GameCamera.h"

#include <numbers>
#include "imgui.h"

#include "Input.h"
#include "CameraManager.h"

#include "gameScene/player/Player.h"

void GameCamera::Init()
{
	JsonInit();

	// カメラの初期化
	mainCamera_ = std::make_unique<Camera>();
	mainCamera_->Initialize();
	mainCamera_->SetRotation(data_.Get<Vector3>("mainRotate"));
	Vector3 translation = data_.Get<Vector3>("mainPosition");
	mainCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(mainCamera_.get());
	CameraManager::GetInstance()->SetActiveCamera(1);
	mainCamera_->Update();

	sabCamera_ = std::make_unique<Camera>();
	sabCamera_->Initialize();
	translation = data_.Get<Vector3>("sabPosition");
	sabCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(sabCamera_.get());
	sabCamera_->Update();
}

void GameCamera::JsonInit()
{
	data_.Init("GameCamera");

	if (!data_.Load()) {
		data_.Set("mainRotate", Vector3{});
		data_.Set("mainPosition", Vector3{});
		data_.Set("sabPosition", Vector3{});
	}
}

void GameCamera::SaveJson()
{
}

void GameCamera::ValueImGui()
{
	ImGui::Begin("GameCamera");
	data_.DrawImGui();
	if (ImGui::Button("Save")) {
		data_.Save();
	}
	ImGui::End();
}

void GameCamera::Update()
{
	Input* input = Input::GetInstance();
	//input->SetGamepadStickDeadzoneScale(0.4f);
	/*if (input->TriggerKey(DIK_1)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}
	if (input->TriggerKey(DIK_2)) {
		CameraManager::GetInstance()->SetActiveCamera(2);
	}*/

	ValueImGui();

	// オフセットの回転角
	const Vector3 offsetRotation = data_.Get<Vector3>("mainRotate");
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
	Vector2 R_StickDire = { input->GetGamepadRightStickX(),input->GetGamepadRightStickY() };
	Vector3 translation = data_.Get<Vector3>("mainPosition") + Vector3{ R_StickDire.x,0.0f,R_StickDire.y };
	Vector3 previous = mainCamera_->GetTranslation();
	Vector3 current = player_->GetTransform().translation_ + translation;

	previous = Vector3::Lerp(previous, current + shakeOffset, 0.1f);
	mainCamera_->SetTranslation(previous);

	// サブカメラの更新
	SabUpdate(shakeOffset);
}

void GameCamera::SabUpdate(const Vector3& shakeOffset)
{
	// プレイヤーの位置と回転
	Vector3 playerPos = player_->GetTransform().translation_;
	Quaternion playerRot = Quaternion::IdentityQuaternion();

	// オフセット（プレイヤーの後方、例：Z方向-10など）
	Vector3 offset = data_.Get<Vector3>("sabPosition");

	// プレイヤーの回転を適用したオフセット
	Matrix4x4 rotMat = Quaternion::MakeRotateMatrix(playerRot);
	Vector3 rotatedOffset = Vector3::TransformNormal(offset, rotMat);

	// カメラの位置は、プレイヤー位置 + 回転されたオフセット
	Vector3 cameraPos = playerPos + rotatedOffset;
	cameraPos = Vector3::Lerp(cameraPos, cameraPos + shakeOffset, 0.1f);
	sabCamera_->SetTranslation(cameraPos);

	// プレイヤーを見つめる
	sabCamera_->SetLookAt(cameraPos, playerPos);

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