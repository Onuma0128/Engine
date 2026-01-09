#include "GameCamera.h"

#include <numbers>
#include "imgui.h"

#include "Input.h"
#include "Easing.h"
#include "CameraManager.h"
#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

void GameCamera::Init()
{
	// 調整項目の初期化
	items_ = std::make_unique<CameraAdjustItem>();
	items_->LoadItems();
	const auto& data = items_->GetCameraData();

	// カメラの初期化
	mainCamera_ = std::make_shared<Camera>();
	mainCamera_->Initialize();
	mainCamera_->SetRotation(data.mainRotate);
	Vector3 translation = data.mainPosition;
	mainCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(mainCamera_);
	CameraManager::GetInstance()->SetActiveCamera(0);
	mainCamera_->Update();

	sabCamera_ = std::make_shared<Camera>();
	sabCamera_->Initialize();
	translation = data.sabPosition;
	sabCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(sabCamera_);
	sabCamera_->Update();

	// カメラの初期化
	bossCamera_ = std::make_shared<Camera>();
	bossCamera_->Initialize();
	bossCamera_->SetRotation(data.bossStartRotate);
	translation = data.bossStartPosition;
	bossCamera_->SetTranslation(translation);
	CameraManager::GetInstance()->SetCamera(bossCamera_);
	bossCamera_->Update();
}

void GameCamera::Update()
{
	Input* input = Input::GetInstance();

#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	// プレイヤーが死んだらカメラを切り替え
	if (playerIsAlive_ && !player_->GetIsAlive()) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}

	// オフセットの回転角
	const auto& data = items_->GetCameraData();
	const Vector3 offsetRotation = data.mainRotate;
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
	Vector3 translation = data.mainPosition + Vector3{ R_StickDire.x,0.0f,R_StickDire.y };
	Vector3 previous = mainCamera_->GetTranslation();
	Vector3 current = player_->GetTransform().translation_ + translation + mainCameraAddPos_;

	previous = Vector3::Lerp(previous, current + shakeOffset, 0.1f);
	mainCamera_->SetTranslation(previous);

	// サブカメラの更新
	SabUpdate(shakeOffset);

	// サブカメラの更新
	BossUpdate(shakeOffset);

	playerIsAlive_ = player_->GetIsAlive();
}

void GameCamera::SabUpdate(const Vector3& shakeOffset)
{
	// データを取得する
	const auto& data = items_->GetCameraData();

	// プレイヤーの位置と回転
	Vector3 playerPos = player_->GetTransform().translation_;
	Quaternion playerRot = Quaternion::IdentityQuaternion();
	sabAnima_.isRotate = data.isSabRotate;
	sabAnima_.rotateSpeed = data.sabRotateSpeed;
	sabAnima_.radius = data.sabRadius;
	sabAnima_.positionY = data.sabPosY;

	// オフセット（プレイヤーの後方）
	if (!sabAnima_.isRotate) {
		sabAnima_.sabCameraOffset = data.sabPosition;
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

void GameCamera::BossUpdate(const Vector3& shakeOffset)
{
	// データを取得
	const auto& data = items_->GetCameraData();

	// タイムを加算
	if (bossCameraTime_ >= 0.0f) {
		bossCameraTime_ += DeltaTimer::GetDeltaTime();
	// キル数が一定以上になったら
	} else {
		uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
		if (spawner_->GetKnockdownCount() >= clearKill) {
			bossCameraTime_ = 0.0f;
		}
	}

	switch (bossCameraState_)
	{
	case GameCamera::BossCameraState::Startup:
	{
		// 待機時間が終了したらカメラを切り替える
		if (bossCameraTime_ > data.bossStartupTime) {
			CameraManager::GetInstance()->SetActiveCamera(2);
			bossCameraState_ = BossCameraState::Active;
			bossCameraTime_ = 0.0f;
		}
	}
		break;
	case GameCamera::BossCameraState::Active:
	{
		float t = bossCameraTime_ / data.bossActiveTime;
		t = Easing::EaseInQuint(std::clamp(t, 0.0f, 1.0f));
		Vector3 rotation = Vector3::Lerp(data.bossStartRotate, data.bossEndRotate, t);
		Vector3 translate = Vector3::Lerp(data.bossStartPosition, data.bossEndPosition, t);
		Vector3 bossPos = { boss_->GetTransform().translation_ };
		translate += Vector3{ bossPos.x,0.0f,bossPos.z };
		translate = Vector3::Lerp(translate, translate + shakeOffset, 0.1f);
		// 回転と座標を更新
		bossCamera_->SetRotation(rotation);
		bossCamera_->SetTranslation(translate);

		if (bossCameraTime_ > data.bossActiveTime) {
			preBossCameraPosition_ = bossCamera_->GetTranslation();
			bossCameraState_ = BossCameraState::Recover;
			bossCameraTime_ = 0.0f;
		}
	}
		break;
	case GameCamera::BossCameraState::Recover:
	{
		Vector3 translate = preBossCameraPosition_;
		translate = Vector3::Lerp(translate, translate + shakeOffset, 0.1f);
		// 座標を更新
		bossCamera_->SetTranslation(translate);

		// 待機時間が終了したらカメラを切り替える
		if (bossCameraTime_ > data.bossRecoverTime) {
			CameraManager::GetInstance()->SetActiveCamera(0);
			bossCameraState_ = BossCameraState::End;
			bossCameraTime_ = 0.0f;
		}
	}
		break;
	case GameCamera::BossCameraState::End:
		break;
	default:
		break;
	}
}
