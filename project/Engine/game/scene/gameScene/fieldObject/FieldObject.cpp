#include "FieldObject.h"

void FieldObject::Init(SceneObject object)
{
	Object3d::Initialize(object.fileName);
	Object3d::SetSceneRenderer();
	transform_ = object.transform;
	centerPosition_ = transform_.translation_;

	if (object.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = "FieldObject";
		Collider::myType_ = object.collider.type;
		Collider::offsetPosition_ = object.collider.center;
		Collider::size_ = object.collider.size;
		Collider::DrawCollider();
	}

	effect_ = std::make_unique<FieldObjectEffect>();
	effect_->Init();
}

void FieldObject::Update()
{
	// シェイクを更新する
	UpdateShake(shake_);

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
		shake_ = { 5.0f,0.0f,5.0f };
		// 弾が飛んできた方向を取得
		Matrix4x4 rotate = Quaternion::MakeRotateMatrix(other->GetRotate());
		Vector3 velocity = Vector3::ExprUnitZ.Transform(rotate);
		// エフェクトを描画
		WorldTransform transform;
		transform.rotation_ = transform_.rotation_;
		transform.translation_ = transform_.translation_ - (velocity * 0.2f);
		effect_->OnceWoodChipEffect(transform);
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
	shake = shake * 0.9f;
}
