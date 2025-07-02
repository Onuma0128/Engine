#include "PlayerMoveState.h"

#include "WinApp.h"
#include "CameraManager.h"
#include "Input.h"
#include "DeltaTimer.h"
#include "Camera.h"

#include "gameScene/player/Player.h"
#include "PlayerAvoidState.h"
#include "PlayerSpecialMoveState.h"

PlayerMoveState::PlayerMoveState(Player* player) :PlayerBaseState(player) {}

void PlayerMoveState::Init()
{
	rightStickVelocity_ = { 0.0f,0.0f,1.0f };
	rightStickQuaternion_ = Quaternion::IdentityQuaternion();
}

void PlayerMoveState::Finalize()
{
}

void PlayerMoveState::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// 移動の処理
	const float playerSpeed = player_->GetItem()->GetPlayerData().speed;
	Vector3 velocity = CreateVelocity();
	player_->GetTransform().translation_ += velocity * DeltaTimer::GetDeltaTime() * playerSpeed;

	// 移動時の回転の処理
	if (velocity.Length() != 0.0f) {
		// 回転を適応
		player_->GetTransform().rotation_ = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, velocity, 0.1f);
		// 移動時のエフェクト
		player_->GetEffect()->OnceMoveEffect();
	}

	// 右のスティックのvelocityを取得
	velocity.x = input->GetGamepadRightStickX();
	velocity.z = input->GetGamepadRightStickY();
	// 右のスティックの入力があれば
	if (velocity.Length() != 0.0f) {
		player_->SetIsPlayingMouse(false);

	// 右のスティックの入力が無ければ
	} else {
		// マウスを取得するか
		if (input->TriggerMouseButton(0) && !player_->GetIsPlayingMouse()) {
			player_->SetIsPlayingMouse(true);

		} else if (player_->GetIsPlayingMouse()) {
			// マウス取得開始しているなら
			Vector2 position = {};
			position.x = static_cast<float>(input->GetMousePosX());
			position.y = static_cast<float>(input->GetMousePosY());
			Vector3 ndc = {
				(position.x / WinApp::kClientWidth) * 2.0f - 1.0f,
				-((position.y / WinApp::kClientHeight) * 2.0f - 1.0f),
				1.0f 
			};
			// ワールド座標に変換
			Matrix4x4 invVP = Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix());
			Vector3 nearPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 0.0f), invVP);
			Vector3 farPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 1.0f), invVP);
			// y軸が0の座標の位置を求める
			Vector3 dir = farPos - nearPos;
			float denom = dir.y;
			Vector3 hitPos = {};

			// 地面と平行か計算
			const float EPS = 1e-6;
			if (std::fabs(denom) > EPS) {
				float planeY = 0.5f;
				float t = (planeY - nearPos.y) / denom;
				if (t >= 0.0f) {
					hitPos = nearPos + dir * t;
				}
			}
			// ベクトルを算出
			velocity = hitPos - player_->GetTransform().translation_;
			velocity.y = 0.0f;
			if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
		}
	}
	if (velocity.Length() != 0.0f) {
		// StickのVelocityから回転を計算
		rightStickVelocity_ = velocity;
		rightStickQuaternion_ = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rightStickVelocity_, 1.0f);
		Quaternion target = Quaternion::Slerp(player_->GetRightStickQua(), rightStickQuaternion_, 0.3f);
		player_->SetRightStickQua(target);
	}


	// 弾を発射する(弾を発射するとリロードが止まる)
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || input->TriggerMouseButton(0)) {
		isReloadBullet_ = false;
		reloadBulletTime_ = 0.0f;
		reloadStartTime_ = 0.0f;
		player_->AttackBullet();
	}
	// 弾のリロードを開始する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || input->TriggerMouseButton(1)) {
		isReloadBullet_ = true;
	}
	// リロードをする関数
	ReloadBullet();

	// 避けの状態に遷移
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_B) || input->TriggerKey(DIK_LSHIFT)) {
		player_->GetEffect()->OnceAvoidEffect();
		player_->ChengeState(std::make_unique<PlayerAvoidState>(player_));
		return;
	}

	// 必殺技の状態に遷移
	if ((input->GetGamepadLeftTrigger() != 0.0f || input->TriggerKey(DIK_SPACE)) && 
		!player_->GetEffect()->GetIsSpecialMove()) {
		player_->GetEffect()->SetIsSpecialMove(true);
		player_->ChengeState(std::make_unique<PlayerSpecialMoveState>(player_));
		return;
	}
}

void PlayerMoveState::Draw()
{
}

void PlayerMoveState::ReloadBullet()
{
	if (isReloadBullet_) {

		// リロード開始時間を過ぎているかチェック
		reloadStartTime_ += DeltaTimer::GetDeltaTime();
		if (reloadStartTime_ < player_->GetItem()->GetBulletData().reloadStartTime) { return; }

		// 前の時間を取得
		int32_t previousTime = static_cast<int32_t>(reloadBulletTime_);

		// 今の時間を更新
		reloadBulletTime_ += DeltaTimer::GetDeltaTime() / player_->GetItem()->GetBulletData().reloadTime;
		int32_t currentTime = static_cast<int32_t>(reloadBulletTime_);

		// 1秒立ったらリロードをする
		if (previousTime != currentTime) {
			player_->ReloadBullet();
		}
	}
}

const Vector3 PlayerMoveState::CreateVelocity() 
{
	// Inputを取得
	Input* input = Input::GetInstance();
	Vector3 velocity{};

	velocity.x = input->GetGamepadLeftStickX();
	velocity.z = input->GetGamepadLeftStickY();
	// 左のスティックの入力が無ければ
	if (velocity.Length() == 0.0f) {
		if (input->PushKey(DIK_W)) {
			velocity.z = 1.0f;
		}
		if (input->PushKey(DIK_A)) {
			velocity.x = -1.0f;
		}
		if (input->PushKey(DIK_S)) {
			velocity.z = -1.0f;
		}
		if (input->PushKey(DIK_D)) {
			velocity.x = 1.0f;
		}
	}

	if (velocity.Length() != 0.0f) { return velocity.Normalize(); }
	return velocity;
}