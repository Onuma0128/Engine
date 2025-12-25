#include "FieldObject.h"

#include <numbers>

#include "DeltaTimer.h"
#include "Easing.h"

#include "SearchAlgorithm/Collision/MapCollision.h"
#include "GameCamera/GameCamera.h"

void FieldObject::Init(SceneObject object)
{
	Object3d::Initialize(object.fileName);
	Object3d::SetSceneRenderer();
	if (object.tag != "ground") {
		Object3d::GetMaterial().outlineMask = true;
		Object3d::GetMaterial().outlineColor = Vector3::ExprZero;
	}
	transform_ = object.transform;
	centerPosition_ = transform_.translation_;

	if (object.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = object.tag;
		Collider::myType_ = object.collider.type;
		Collider::offsetPosition_ = object.collider.center;
		Collider::size_ = object.collider.size;
		Collider::radius_ = object.collider.radius;
		Collider::targetColliderName_ = {
			"Player","PlayerBullet","PlayerBulletSpecial","MuscleCompanion",
			"EnemyRanged","EnemyRangedElite","EnemyRay","PlayerShotRay"
		};
		Collider::DrawCollider();
	}

	effect_ = std::make_unique<FieldObjectEffect>();
	effect_->Init(object.tag);
}

void FieldObject::Update()
{
	// シェイクを更新する
	UpdateShake(shake_);

	// 倒れる処理を更新する
	UpdateBreak();

	effect_->Update();

	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}

void FieldObject::OnCollisionEnter(Collider* other)
{
	// 弾が当たったら
	if (other->GetColliderName() == "PlayerBullet" || other->GetColliderName() == "PlayerBulletSpecial" ||
		other->GetColliderName() == "EnemyRanged" || other->GetColliderName() == "EnemyRangedElite") {
		// シェイクさせる
		shake_ = items_->GetMainData().shakeOffset;
		if (Collider::colliderName_ == "DeadTree" || Collider::colliderName_ == "fence") {
			// 弾が飛んできた方向を取得
			Matrix4x4 rotate = Quaternion::MakeRotateMatrix(other->GetRotate());
			Vector3 velocity = Vector3::ExprUnitZ.Transform(rotate);
			// エフェクトを描画
			WorldTransform transform;
			transform.rotation_ = Quaternion::IdentityQuaternion();
			transform.translation_ = other->GetCenterPosition() + (velocity * 0.2f);
			effect_->OnceHitEffect(transform);
			if (Collider::colliderName_ == "DeadTree" && !isBreak_) {
				// 木の回転を計算する
				const float angle = std::numbers::pi_v<float> / 2.1f;
				breakRotate_ = other->GetRotate() * Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, angle);
				prevRotate_ = other->GetRotate();
				transform_.rotation_ = other->GetRotate();
				isBreak_ = true;
				// マップの衝突判定を更新する
				float prevRotate = Quaternion::ToEuler(Collider::GetRotate()).y;
				float currentRotate = Quaternion::ToEuler(other->GetRotate()).y;
				OBB_2D prevOBB{
					.center = { Collider::centerPosition_.x,Collider::centerPosition_.z },
					.rotate = prevRotate,
					.size = { Collider::size_.x,Collider::size_.z }
				};
				Vector3 offset = Collider::offsetPosition_.Transform(Quaternion::MakeRotateMatrix(breakRotate_));
				OBB_2D currentOBB{
					.center = { Collider::centerPosition_.x + offset.x,Collider::centerPosition_.z + offset.z },
					.rotate = currentRotate,
					.size = { Collider::size_.x,Collider::size_.y } 
				};
				mapCollision_->ReTargetMapCollisionOBB(prevOBB, currentOBB);
				// カメラをシェイクさせる
				gameCamera_->SetShake(items_->GetMainData().cameraShakePower);
				// PlayerShotRayを無効化
				Collider::targetColliderName_[6] = "";
			}
		}
	}
}

void FieldObject::OnCollisionStay(Collider* other)
{
}

void FieldObject::OnCollisionExit(Collider* other)
{
}

float FieldObject::RandomRange(float value)
{
	int v = static_cast<int>(value + 1);
	value = static_cast<float>(rand() % v - (v / 2)) * 0.1f;
	return value;
}

void FieldObject::UpdateShake(Vector3& shake)
{
	if (shake.Length() <= 0.01f) { return; }

	Vector3 offset{
		RandomRange(shake.x),
		0.0f,
		RandomRange(shake.z),
	};

	transform_.translation_ = centerPosition_ + offset;
	shake = shake * items_->GetMainData().shakePower;
}

void FieldObject::UpdateBreak()
{
	if (isBreak_ && breakTimer_ < 1.0f) {
		const auto& data = items_->GetMainData();
		breakTimer_ += DeltaTimer::GetDeltaTime() / data.breakTimer;
		breakTimer_ = std::clamp(breakTimer_, 0.0f, 1.0f);
		float t = Easing::EaseOutBounce(breakTimer_);
		transform_.rotation_ = Quaternion::Slerp(prevRotate_, breakRotate_,t);
		centerPosition_.y = breakTimer_ * 0.75f;
		if (t > data.breakEmitTimer) {
			effect_->OnceBreakEffect(transform_);
		}
	}
}
