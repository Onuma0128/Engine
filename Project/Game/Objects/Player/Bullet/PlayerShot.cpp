#include "PlayerShot.h"

#include "Input.h"

#include "objects/player/Player.h"
#include "objects/player/adjustItem/PlayerAdjustItem.h"

void PlayerShot::Init(Player* player)
{
	player_ = player;

	rightStickQuaternion_ = Quaternion::IdentityQuaternion();

	// プレイヤーの目線のコライダーを初期化
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kOBB;
	Collider::colliderName_ = "PlayerShotRay";
	Collider::isActive_ = true;
	Collider::targetColliderName_ = { "Enemy","BossEnemy" };
	Collider::DrawCollider();

	rayReticle_ = std::make_unique<PlayerRayReticle>();
	rayReticle_->Init();
}

void PlayerShot::Update()
{
	RayUpdate();
}

void PlayerShot::DrawUI()
{
	rayReticle_->Draw();
}


void PlayerShot::ReloadBullet()
{
	isGatherRequested_ = true;
}

void PlayerShot::AttackBullet()
{
	isShot_ = true;
}

void PlayerShot::OnCollisionStay(Collider* other)
{
	isRayHit_ = true;

	if (!rayHitCollider_) {
		rayHitCollider_ = other;
	} else {
		float distanceNow = Vector3::Distance(player_->GetTransform().translation_, rayHitCollider_->GetCenterPosition());
		float distanceOther = Vector3::Distance(player_->GetTransform().translation_, other->GetCenterPosition());
		if (distanceOther < distanceNow) {
			rayHitCollider_ = other;
		}
	}
}

void PlayerShot::RayUpdate()
{
	// 右スティックの入力を取得
	Vector3 rotateVelocity{};
	Input::GetInstance()->SetGamepadStickDeadzoneScale(0.5f);
	rotateVelocity.x = Input::GetInstance()->GetGamepadRightStickX();
	rotateVelocity.z = Input::GetInstance()->GetGamepadRightStickY();
	Input::GetInstance()->SetGamepadStickDeadzoneScale(1.0f);

	// 右スティックの入力があるならその方向にRayを向ける
	Quaternion rightQuaternion = Quaternion::IdentityQuaternion();
	if (rotateVelocity.Length() > 0.01f) {
		rayDirection_ = rotateVelocity;
		Collider::isActive_ = true;
	} else {
		Collider::isActive_ = false;
	}
	rightQuaternion = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rayDirection_, 0.3f);
	const auto& itemData = player_->GetItem()->GetPreObjectData();
	const auto rotateMatrix = Quaternion::MakeRotateMatrix(rightQuaternion);
	auto centerPosition = player_->GetTransform().translation_ + itemData.rayColliderPosition.Transform(rotateMatrix);

	// コライダーの更新
	Collider::size_ = itemData.rayColliderSize;
	Collider::rotate_ = rightQuaternion;
	Collider::centerPosition_ = centerPosition;
	Collider::Update();

	rayReticle_->SetRaticleAlpha(isRayHit_);
	if (rayHitCollider_) {
		rayReticle_->SetPosition(rayHitCollider_->GetCenterPosition());
	}
	rayReticle_->Update();

	ResetRayHit();
}
