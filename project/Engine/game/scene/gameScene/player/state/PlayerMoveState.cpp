#include "PlayerMoveState.h"

#include "WinApp.h"
#include "CameraManager.h"
#include "Input.h"
#include "DeltaTimer.h"
#include "Camera.h"

#include "gameScene/player/Player.h"
#include "gameScene/player/bullet/PlayerShot.h"
#include "PlayerAvoidState.h"
#include "PlayerSpecialMoveState.h"

PlayerMoveState::PlayerMoveState(Player* player) :PlayerBaseState(player) {}

void PlayerMoveState::Init()
{
	rightStickQuaternion_ = Quaternion::IdentityQuaternion();

	Vector3 moveVelocity = CreateMoveVelocity();

	// Inputを取得
	Input* input = Input::GetInstance();
	// 右のスティックのvelocityを取得
	Vector3 rotateVelocity{};
	rotateVelocity.x = input->GetGamepadRightStickX();
	rotateVelocity.z = input->GetGamepadRightStickY();
	// 右のスティックの入力が無ければ
	if (rotateVelocity.Length() == 0.0f) {
		if (player_->GetIsPlayingMouse()) {
			rotateVelocity = CreateRotateVelocity();
		} else {
			rotateVelocity = Vector3::ExprUnitZ;
		}
	}

	// アニメーションを逆再生する
	if (moveVelocity.Length() != 0.0f) {
		// 向けている方向でアニメーションを決める
		float dot = Vector3::Dot(moveVelocity, rotateVelocity);
		if (dot <= 0.0f) {
			isReversePlay_ = false;
			chengeAniamtion_ = false;
		} else {
			isReversePlay_ = true;
			chengeAniamtion_ = false;
		}

	} else {
		player_->GetReversePlay() = false;
		chengeAniamtion_ = true;
	}
}

void PlayerMoveState::Finalize()
{
}

void PlayerMoveState::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// 後ろ歩きしているか
	float speed = 0.0f;
	if (isReversePlay_) { speed = player_->GetItem()->GetPlayerData().backSpeed; }
	else { speed = player_->GetItem()->GetPlayerData().speed; }

	// 移動の処理
	const float playerSpeed = speed;
	Vector3 moveVelocity = CreateMoveVelocity();
	player_->GetTransform().translation_ += moveVelocity * DeltaTimer::GetDeltaTime() * playerSpeed;

	// 右のスティックのvelocityを取得
	Vector3 rotateVelocity{};
	rotateVelocity.x = input->GetGamepadRightStickX();
	rotateVelocity.z = input->GetGamepadRightStickY();
	// 右のスティックの入力があれば
	if (rotateVelocity.Length() != 0.0f) {
		player_->SetIsPlayingMouse(false);

	// 右のスティックの入力が無ければ
	} else {
		// マウスを取得するか
		if (input->TriggerMouseButton(0) && input->PushMouseButton(1) && !player_->GetIsPlayingMouse()) {
			player_->SetIsPlayingMouse(true);

		} else if (player_->GetIsPlayingMouse()) {
			// マウスからのVelocity算出
			rotateVelocity = CreateRotateVelocity();
		} else {
			rotateVelocity = moveVelocity;
		}
	}

	// StickのVelocityから回転を計算
	if (rotateVelocity.Length() != 0.0f) {
		rightStickVelocity_ = rotateVelocity;
		rightStickQuaternion_ = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rightStickVelocity_, 1.0f);
		Quaternion target = Quaternion::Slerp(player_->GetShot()->GetRightStickQua(), rightStickQuaternion_, 0.3f);
		player_->GetShot()->SetRightStickQua(target);
		// 回転を適応
		player_->GetTransform().rotation_ = target;
	}

	// アニメーションを逆再生する
	if (moveVelocity.Length() != 0.0f) {
		// エフェクトを出す
		player_->GetEffect()->OnceMoveEffect();
		// 向けている方向でアニメーションを決める
		float dot = Vector3::Dot(moveVelocity, rightStickVelocity_);
		if (dot <= -0.5f) {
			if ((!isReversePlay_ || !chengeAniamtion_) && player_->PlayByName("Walk_Gun")) {
				player_->GetReversePlay() = true;
				isReversePlay_ = true;
				chengeAniamtion_ = true;
			}
		} else {
			if ((isReversePlay_ || !chengeAniamtion_) && player_->PlayByName("Run_Gun")) {
				player_->GetReversePlay() = false;
				isReversePlay_ = false;
				chengeAniamtion_ = true;
			}
		}

	} else {
		if (chengeAniamtion_) {
			if (player_->PlayByName("Idle_Gun")) {
				player_->GetReversePlay() = false;
				chengeAniamtion_ = false;
			}
		}
	}

	// アクションを起こす
	SomeAction();
}

void PlayerMoveState::Draw()
{
}

void PlayerMoveState::SomeAction()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// 弾を発射する(弾を発射するとリロードが止まる)
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) ||
		(input->TriggerMouseButton(0) && player_->GetIsPlayingMouse())) {
		isReloadBullet_ = false;
		reloadBulletTime_ = 0.0f;
		reloadStartTime_ = 0.0f;
		player_->GetShot()->AttackBullet();
	}
	// 弾のリロードを開始する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) ||
		(input->TriggerMouseButton(1) && player_->GetIsPlayingMouse())) {
		isReloadBullet_ = true;
	}
	// リロードをする関数
	ReloadBullet();

	// 避けの状態に遷移
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_B) ||
		(input->TriggerKey(DIK_LSHIFT) && player_->GetIsPlayingMouse())) {
		player_->GetEffect()->OnceAvoidEffect();
		player_->GetReversePlay() = false;
		player_->ChengeState(std::make_unique<PlayerAvoidState>(player_));
		return;
	}

	// 必殺技の状態に遷移
	if ((input->GetGamepadLeftTrigger() != 0.0f ||
		(input->TriggerKey(DIK_SPACE) && player_->GetIsPlayingMouse())) &&
		!player_->GetEffect()->GetIsSpecialMove()) {
		player_->GetEffect()->SetIsSpecialMove(true);
		player_->ChengeState(std::make_unique<PlayerSpecialMoveState>(player_));
		return;
	}
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
			player_->GetShot()->ReloadBullet();
		}
	}
}

const Vector3 PlayerMoveState::CreateMoveVelocity()
{
	// Inputを取得
	Input* input = Input::GetInstance();
	Vector3 velocity{};

	velocity.x = input->GetGamepadLeftStickX();
	velocity.z = input->GetGamepadLeftStickY();
	// 左のスティックの入力が無ければ
	if (velocity.Length() == 0.0f && player_->GetIsPlayingMouse()) {
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

const Vector3 PlayerMoveState::CreateRotateVelocity()
{
	// Inputを取得
	Input* input = Input::GetInstance();

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
	const float EPS = 1e-6f;
	if (std::fabs(denom) > EPS) {
		float planeY = 1.5f;
		float t = (planeY - nearPos.y) / denom;
		if (t >= 0.0f) {
			hitPos = nearPos + dir * t;
		}
	}
	// ベクトルを算出
	Vector3 rotateVelocity{};
	rotateVelocity = hitPos - player_->GetTransform().translation_;
	rotateVelocity.y = 0.0f;
	if (rotateVelocity.Length() != 0.0f) { return rotateVelocity.Normalize(); }
	return rotateVelocity;
}
