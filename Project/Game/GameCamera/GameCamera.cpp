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
#include "Objects/MuscleCompanion/Manager/MuscleCompanionManager.h"

void GameCamera::Init()
{
	// 調整項目の初期化
	items_ = std::make_unique<CameraAdjustItem>();
	items_->LoadItems();
	const auto& data = items_->GetCameraData();

	// カメラの初期化
	CameraManager::GetInstance()->Clear();
    mainCamera_ = std::make_shared<NumaEngine::Camera>();
	mainCamera_->Initialize();
	mainCamera_->SetRotation(data.mainRotate);
    NumaEngine::Vector3 translation = data.mainPosition;
	mainCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(mainCamera_);
	CameraManager::GetInstance()->SetActiveCamera(0);
	mainCamera_->Update();

    sabCamera_ = std::make_shared<NumaEngine::Camera>();
	sabCamera_->Initialize();
	translation = data.sabPosition;
	sabCamera_->SetTranslation(translation + player_->GetTransform().translation_);
	CameraManager::GetInstance()->SetCamera(sabCamera_);
	sabCamera_->Update();

	// カメラの初期化
    bossCamera_ = std::make_shared<NumaEngine::Camera>();
	bossCamera_->Initialize();
	bossCamera_->SetRotation(data.bossStartRotate);
	translation = data.bossStartPosition;
	bossCamera_->SetTranslation(translation);
	CameraManager::GetInstance()->SetCamera(bossCamera_);
	bossCamera_->Update();
}

void GameCamera::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	// プレイヤーが死んだらカメラを切り替え
	if (playerIsAlive_ && !player_->GetIsAlive()) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}

	// シェイクオフセット（ランダムな微小ノイズ）
    NumaEngine::Vector3 shakeOffset{};
    if (shakeStrength_ > 0.01f) {
        shakeOffset = {
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shakeStrength_,
        };
        shakeStrength_ *= shakeDecay_; // 減衰させる
    }

	// メインカメラの更新
	mainUpdate(shakeOffset);

	// サブカメラの更新
	SabUpdate(shakeOffset);

	// ボスカメラの更新
	BossUpdate(shakeOffset);

	playerIsAlive_ = player_->GetIsAlive();
}

void GameCamera::mainUpdate(const NumaEngine::Vector3& shakeOffset)
{
	// データを取得する
	const auto& data = items_->GetCameraData();

	// タイムを加算
	if (clearCameraTime_ >= 0.0f) {
		if (clearCameraTime_ == 0.0f) {
			if (data.clearShackIndex == clearDataIndex_) {
				shakeStrength_ = data.clearShackPow;
			}
		}
        clearCameraTime_ += NumaEngine::DeltaTimer::GetDeltaTime();
	// ボスが死んだら
	} else if (boss_ && boss_->GetBossState() == BossState::Dead) {
		clearCameraTime_ = 0.0f;
		clearDataIndex_ = 0;
	}

	// ボスが死んだらカメラ操作をする
	bool isDataSize = data.clearData.size() > clearDataIndex_ + 1;
	if (boss_ && boss_->GetBossState() == BossState::Dead) {
		if (isDataSize) {
			// 現在のインデックス
			int idx = static_cast<int>(clearDataIndex_);
			int maxIdx = static_cast<int>(data.clearData.size()) - 1;

			// Catmull-Romに必要な4点（P0, P1, P2, P3）のインデックスを計算
			// P1(始点) と P2(終点) が現在の区間。P0とP3は曲線の制御用。
			// 配列外に出ないよう clamp する
			int i0 = std::clamp(idx - 1, 0, maxIdx);
			int i1 = std::clamp(idx, 0, maxIdx);
			int i2 = std::clamp(idx + 1, 0, maxIdx);
			int i3 = std::clamp(idx + 2, 0, maxIdx);

			// 4点のデータを取得
			const auto& p0 = data.clearData[i0];
			const auto& p1 = data.clearData[i1];
			const auto& p2 = data.clearData[i2];
			const auto& p3 = data.clearData[i3];

			// 進行度 t の計算（現在の区間 P1->P2 にかかる時間を使用）
			float segmentDuration = p1.time;
			// ゼロ除算対策（念のため）
			if (segmentDuration <= 0.0f) { segmentDuration = 0.001f; }

			float t = clearCameraTime_ / segmentDuration;
			t = std::clamp(t, 0.0f, 1.0f);

			// 次の区間へ
			if (t >= 1.0f) {
				clearCameraTime_ = 0.0f;
				++clearDataIndex_;
			}

            NumaEngine::Vector3 centerPos = companionManager_->CompanionCenterPosition();

			// 回転の補間
            NumaEngine::Vector3 clearRotate = NumaEngine::Vector3::CatmullRomInterpolation(
				p0.rotate, p1.rotate, p2.rotate, p3.rotate, t
			);
			// 位置の補間
            NumaEngine::Vector3 localPos = NumaEngine::Vector3::CatmullRomInterpolation(
				p0.position, p1.position, p2.position, p3.position, t
			);
			NumaEngine::Vector3 clearPos = localPos + centerPos;
			NumaEngine::Vector3 prePos = mainCamera_->GetTranslation();
			mainCamera_->SetRotation(clearRotate);
			mainCamera_->SetTranslation(NumaEngine::Vector3::Lerp(prePos, clearPos + shakeOffset, 0.1f));
		} else {
			isClearCameraEnd_ = true;
		}
		return;
	}
	// オフセットの回転角
    const NumaEngine::Vector3 offsetRotation = data.mainRotate;
	// 回転を更新
	mainCamera_->SetRotation(offsetRotation);
	// カメラの回転に合わせた座標を更新
	Input* input = Input::GetInstance();
    NumaEngine::Vector2 R_StickDire = { input->GetGamepadRightStickX(),input->GetGamepadRightStickY() };
    NumaEngine::Vector3 translation = data.mainPosition + NumaEngine::Vector3{ R_StickDire.x,0.0f,R_StickDire.y };
	NumaEngine::Vector3 previous = mainCamera_->GetTranslation();
	NumaEngine::Vector3 current = player_->GetTransform().translation_ + translation + mainCameraAddPos_;

	previous = NumaEngine::Vector3::Lerp(previous, current + shakeOffset, 0.1f);
	mainCamera_->SetTranslation(previous);
}

void GameCamera::SabUpdate(const NumaEngine::Vector3& shakeOffset)
{
	// データを取得する
	const auto& data = items_->GetCameraData();

	// プレイヤーの位置と回転
    NumaEngine::Vector3 playerPos = player_->GetTransform().translation_;
	NumaEngine::Quaternion playerRot = NumaEngine::Quaternion::IdentityQuaternion();
	sabAnima_.isRotate = data.isSabRotate;
	sabAnima_.rotateSpeed = data.sabRotateSpeed;
	sabAnima_.radius = data.sabRadius;
	sabAnima_.positionY = data.sabPosY;

	// オフセット（プレイヤーの後方）
	if (!sabAnima_.isRotate) {
		sabAnima_.sabCameraOffset = data.sabPosition;
	} else {
        sabAnima_.rotateTimer += NumaEngine::DeltaTimer::GetDeltaTime() / sabAnima_.rotateSpeed;
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
    NumaEngine::Matrix4x4 rotMat = NumaEngine::Quaternion::MakeRotateMatrix(playerRot);
	NumaEngine::Vector3 rotatedOffset = NumaEngine::Vector3::TransformNormal(sabAnima_.sabCameraOffset, rotMat);

	// カメラの位置は、プレイヤー位置 + 回転されたオフセット
    NumaEngine::Vector3 cameraPos = playerPos + rotatedOffset;
	cameraPos = NumaEngine::Vector3::Lerp(cameraPos, cameraPos + shakeOffset, 0.1f);
	sabCamera_->SetTranslation(cameraPos);

	// プレイヤーを見つめる
	sabCamera_->SetLookAt(cameraPos, playerPos);

}

void GameCamera::BossUpdate(const NumaEngine::Vector3& shakeOffset)
{
	// データを取得
	const auto& data = items_->GetCameraData();

	// タイムを加算
	if (bossCameraTime_ >= 0.0f) {
        bossCameraTime_ += NumaEngine::DeltaTimer::GetDeltaTime();
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
        NumaEngine::Vector3 rotation = NumaEngine::Vector3::Lerp(data.bossStartRotate, data.bossEndRotate, t);
		NumaEngine::Vector3 translate = NumaEngine::Vector3::Lerp(data.bossStartPosition, data.bossEndPosition, t);
		NumaEngine::Vector3 bossPos = { boss_->GetTransform().translation_ };
		translate += NumaEngine::Vector3{ bossPos.x,0.0f,bossPos.z };
		translate = NumaEngine::Vector3::Lerp(translate, translate + shakeOffset, 0.1f);
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
		NumaEngine::Vector3 translate = preBossCameraPosition_;
		translate = NumaEngine::Vector3::Lerp(translate, translate + shakeOffset, 0.1f);
		// 座標を更新
		bossCamera_->SetTranslation(translate);

		// 待機時間が終了したらカメラを切り替える
		if (bossCameraTime_ > data.bossRecoverTime) {
			CameraManager::GetInstance()->SetActiveCamera(0);
			bossCameraState_ = BossCameraState::End;
			bossCameraTime_ = -1.0f;
		}
	}
		break;
	case GameCamera::BossCameraState::End:
		break;
	default:
		break;
	}
}

void GameCamera::BossCameraReset()
{
	bossCameraTime_ = -1.0f;
	clearCameraTime_ = -1.0f;
	isClearCameraEnd_ = false;
	bossCameraState_ = BossCameraState::Startup;
	CameraManager::GetInstance()->SetActiveCamera(0);
}

void GameCamera::BossCameraEnd()
{
	bossCameraTime_ = -1.0f;
	clearCameraTime_ = -1.0f;
	isClearCameraEnd_ = false;
	bossCameraState_ = BossCameraState::End;
	CameraManager::GetInstance()->SetActiveCamera(0);
}


