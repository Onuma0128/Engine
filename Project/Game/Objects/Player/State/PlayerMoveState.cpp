#include "PlayerMoveState.h"

#include "WinApp.h"
#include "CameraManager.h"
#include "Input.h"
#include "DeltaTimer.h"
#include "Camera.h"

#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Player/Player.h"
#include "Objects/Player/Bullet/PlayerShot.h"
#include "PlayerAvoidState.h"

PlayerMoveState::PlayerMoveState(Player* player) :PlayerBaseState(player) {}

void PlayerMoveState::Init()
{
	rightStickQuaternion_ = NumaEngine::Quaternion::IdentityQuaternion();

	NumaEngine::Vector3 moveVelocity = CreateMoveVelocity();

	// Inputを取得
	Input* input = Input::GetInstance();
	// 右のスティックのvelocityを取得
	NumaEngine::Vector3 rotateVelocity{};
	rotateVelocity.x = input->GetGamepadRightStickX();
	rotateVelocity.z = input->GetGamepadRightStickY();
	// 右のスティックの入力が無ければ
	if (rotateVelocity.Length() == 0.0f) {
		if (player_->GetIsPlayingMouse()) {
			rotateVelocity = CreateRotateVelocity();
		} else {
			rotateVelocity = NumaEngine::Vector3::ExprUnitZ;
		}
	}

	// アニメーションを逆再生する
	if (moveVelocity.Length() != 0.0f) {
		// 向けている方向でアニメーションを決める
		float dot = NumaEngine::Vector3::Dot(moveVelocity, rotateVelocity);
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
	
	if (player_->GetShot()) {
		player_->GetShot()->ResetRayHit();
	}
}

void PlayerMoveState::Finalize()
{
}

void PlayerMoveState::Update()
{
	if (player_->GetBoss()->GetBossState() == BossState::Dead) {
		player_->PlayByName("Idle");
		return;
	}

	// Inputを取得
	Input* input = Input::GetInstance();

	// 後ろ歩きしているか
	float speed = 0.0f;
	if (isReversePlay_) { speed = player_->GetItem()->GetPlayerData().backSpeed; }
	else { speed = player_->GetItem()->GetPlayerData().speed; }

	// 移動の処理
	NumaEngine::Vector3 moveVelocity = CreateMoveVelocity();
	NumaEngine::Vector3 position = player_->GetTransform().translation_;
    player_->SetTransformTranslation(position + moveVelocity * NumaEngine::DeltaTimer::GetDeltaTime() * speed);

	// 右のスティックのvelocityを取得
	NumaEngine::Vector3 rotateVelocity{};
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
		if(player_->GetShot()->GetIsRayHit()) {
			rotateVelocity = (player_->GetShot()->GetRayHitCollider()->GetCenterPosition() - player_->GetTransform().translation_).Normalize();
		}
		rightStickVelocity_ = rotateVelocity;
		rightStickQuaternion_ = NumaEngine::Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rightStickVelocity_, 1.0f);
		NumaEngine::Quaternion target = NumaEngine::Quaternion::Slerp(player_->GetShot()->GetRightStickQua(), rightStickQuaternion_, 0.3f);
		player_->GetShot()->SetRightStickQua(target);
		// 回転を適応
		player_->SetTransformRotation(target);
	}

	// アニメーションを逆再生する
	if (moveVelocity.Length() != 0.0f) {
		// エフェクトを出す
		player_->GetEffect()->OnceMoveEffect();
		// 向けている方向でアニメーションを決める
		float dot = NumaEngine::Vector3::Dot(moveVelocity, rightStickVelocity_);
		if (dot <= -0.5f) {
			if ((!isReversePlay_ || !chengeAniamtion_) && player_->PlayByName("Walk")) {
				player_->GetReversePlay() = true;
				isReversePlay_ = true;
				chengeAniamtion_ = true;
			}
		} else {
			if ((isReversePlay_ || !chengeAniamtion_) && player_->PlayByName("Run")) {
				player_->GetReversePlay() = false;
				isReversePlay_ = false;
				chengeAniamtion_ = true;
			}
		}

	} else {
		if (chengeAniamtion_) {
			if (player_->PlayByName("Idle")) {
				player_->GetReversePlay() = false;
				chengeAniamtion_ = false;
			}
		}
	}

	NumaEngine::Vector2 min = player_->GetItem()->GetPlayerData().minPlayerClamp;
	NumaEngine::Vector2 max = player_->GetItem()->GetPlayerData().maxPlayerClamp;
	position.x = std::clamp(player_->GetTransform().translation_.x, min.x, max.x);
	position.z = std::clamp(player_->GetTransform().translation_.z, min.y, max.y);
	player_->SetTransformTranslation(position);

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
		player_->GetShot()->AttackBullet();
	}
	// 弾のリロードを開始する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) ||
		(input->TriggerMouseButton(1) && player_->GetIsPlayingMouse())) {
		player_->GetShot()->ReloadBullet();
	}

	// 避けの状態に遷移
	if (player_->GetAvoidCoolTimer() > 0.0f) {
        float coolTime = player_->GetAvoidCoolTimer();
		coolTime -= NumaEngine::DeltaTimer::GetDeltaTime();
		player_->SetAvoidCoolTimer(coolTime);
	} else {
		if (input->TriggerGamepadButton(XINPUT_GAMEPAD_B) ||
			(input->TriggerKey(DIK_LSHIFT) && player_->GetIsPlayingMouse())) {
			player_->GetEffect()->OnceAvoidEffect();
			player_->GetReversePlay() = false;
			player_->ChangeState(std::make_unique<PlayerAvoidState>(player_));
			return;
		}
	}
}

const NumaEngine::Vector3 PlayerMoveState::CreateMoveVelocity()
{
	// Inputを取得
	Input* input = Input::GetInstance();
	NumaEngine::Vector3 velocity{};

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

const NumaEngine::Vector3 PlayerMoveState::CreateRotateVelocity()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// マウス取得開始しているなら
	NumaEngine::Vector2 position = {};
	position.x = static_cast<float>(input->GetMousePosX());
	position.y = static_cast<float>(input->GetMousePosY());
    NumaEngine::Vector3 ndc = {
		(position.x / NumaEngine::WinApp::kClientWidth) * 2.0f - 1.0f,
		-((position.y / NumaEngine::WinApp::kClientHeight) * 2.0f - 1.0f),
		1.0f
	};
	// ワールド座標に変換
    NumaEngine::Matrix4x4 invVP = NumaEngine::Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix());
	NumaEngine::Vector3 nearPos = NumaEngine::Vector3::Transform(NumaEngine::Vector3(ndc.x, ndc.y, 0.0f), invVP);
	NumaEngine::Vector3 farPos = NumaEngine::Vector3::Transform(NumaEngine::Vector3(ndc.x, ndc.y, 1.0f), invVP);
	// y軸が0の座標の位置を求める
	NumaEngine::Vector3 dir = farPos - nearPos;
	float denom = dir.y;
	NumaEngine::Vector3 hitPos = {};

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
	NumaEngine::Vector3 rotateVelocity{};
	rotateVelocity = hitPos - player_->GetTransform().translation_;
	rotateVelocity.y = 0.0f;
	if (rotateVelocity.Length() != 0.0f) { return rotateVelocity.Normalize(); }
	return rotateVelocity;
}

