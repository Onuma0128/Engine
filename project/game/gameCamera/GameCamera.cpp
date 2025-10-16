#include "GameCamera.h"

#include <numbers>
#include "imgui.h"

#include "Input.h"
#include "CameraManager.h"
#include "DeltaTimer.h"

#include "objects/player/Player.h"

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
	CameraManager::GetInstance()->SetActiveCamera(0);
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
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("GameCamera")) {
		ImGui::PushItemWidth(150);
		ImGui::Separator();

		data_.DrawImGui();

		if (ImGui::Button("mainCamera")) {
			CameraManager::GetInstance()->SetActiveCamera(0);
		}
		if (ImGui::Button("sabCamera")) {
			CameraManager::GetInstance()->SetActiveCamera(1);
		}
		ImGui::Checkbox("IsSabRotate", &sabAnima_.isRotate);
		ImGui::DragFloat("RotateSpeed", &sabAnima_.rotateSpeed, 0.01f);
		ImGui::DragFloat("SabRadius", &sabAnima_.radius, 0.01f);
		ImGui::DragFloat("SabPosY", &sabAnima_.positionY, 0.01f);

		if (ImGui::Button("Save")) {
			data_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}

void GameCamera::Update()
{
	Input* input = Input::GetInstance();

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
	Vector3 current = player_->GetTransform().translation_ + translation + mainCameraAddPos_;

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

	// オフセット（プレイヤーの後方）
	if (!sabAnima_.isRotate) {
		sabAnima_.sabCameraOffset = data_.Get<Vector3>("sabPosition");
	} else {
		sabAnima_.rotateTimer += DeltaTimer::GetDeltaTime() / sabAnima_.rotateSpeed;
		sabAnima_.rotateTimer = std::clamp(sabAnima_.rotateTimer, 0.0f, 1.0f);
		float t = sabAnima_.rotateTimer * std::numbers::pi_v<float> * 2.0f;
		if (sabAnima_.rotateTimer >= 1.0f) { sabAnima_.rotateTimer = 0.0f; }
		sabAnima_.sabCameraOffset = {
			std::cos(t) * sabAnima_.radius,
			sabAnima_.positionY,
			std::sin(t) * sabAnima_.radius
		};
	}

	// プレイヤーの回転を適用したオフセット
	Matrix4x4 rotMat = Quaternion::MakeRotateMatrix(playerRot);
	Vector3 rotatedOffset = Vector3::TransformNormal(sabAnima_.sabCameraOffset, rotMat);

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