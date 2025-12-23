#include "PlayerBullet.h"

#include "DeltaTimer.h"
#include "Input.h"

#include "Collision/CollisionFilter.h"
#include "Objects/Player/AdjustItem/PlayerAdjustItem.h"
#include "Objects/Player/Bullet/PlayerShot.h"

void PlayerBullet::Init(const std::string& colliderName)
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().enableDraw = false;
	transform_.scale_ = { 0.1f,0.1f ,0.3f };
	transform_.translation_.y = -5.0f;

	isActive_ = false;
	activeTime_ = 0.0f;

	effect_ = std::make_unique<PlayerBulletEffect>();
	effect_->Init();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::kOBB;
	Collider::colliderName_ = colliderName;
	Collider::size_ = transform_.scale_;
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
			"Enemy","EnemyShield",
			"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall"
	};
	Collider::DrawCollider();
}

void PlayerBullet::Update()
{
	BulletData data = item_->GetBulletData();

	// フレームが60立ったらIsActiveをfalseにする
	if (isActive_) {
		activeTime_ += DeltaTimer::GetDeltaTime() * 2.0f;
		if (activeTime_ >= 1.0f) {
			activeTime_ = 1.0f;
			isActive_ = false;
			Collider::isActive_ = false;
			Object3d::GetMaterial().enableDraw = false;
		}
	} 

	// コールバック関数
	if (wasActive_ && !isActive_ && onDeactivatedCallback_) {
		onDeactivatedCallback_();
		effect_->OnceBulletDeleteEffect(transform_);
		transform_.translation_.y = -2.0f;
		Object3d::GetMaterial().enableDraw = false;
	}
	wasActive_ = isActive_;

	// Activeがfalseならこの先を更新しない
	if (!isActive_) {
		Collider::Update();
		Object3d::Update();
		effect_->Update();
		return;
	}

	// 移動処理
	transform_.translation_ += velocity_ * DeltaTimer::GetDeltaTime() * speed_;

	effect_->OnceBulletTrailEffect(transform_);
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform_.rotation_);
	Collider::size_ = transform_.scale_ + data.colliderSize;
	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_ + data.colliderPosition.Transform(rotateMatrix);
	Collider::Update();
	Object3d::Update();
}

void PlayerBullet::OnCollisionEnter(Collider* other)
{
	const auto& name = other->GetColliderName();

	// 敵と当たったらなエフェクトを出す
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName()) || 
		other->GetColliderName() == "EnemyShield") {
		IsCollision();
		effect_->OnceBulletDeleteEffect(transform_);
		transform_.translation_.y = -2.0f;
	}
	if (name == "Enemy") {
		if(Collider::GetColliderName() == "PlayerBullet"){
			IsCollision();
			effect_->OnceBulletDeleteEffect(transform_);
			transform_.translation_.y = -2.0f;
			if(playerShot_->GetChargeCount() <= static_cast<uint32_t>(item_->GetBulletData().maxChargeCount_sp)){
				playerShot_->AddChargeCount();
			}
		}
		playerShot_->AddNockdownCount();
		Input::GetInstance()->Vibrate(0.4f, 0.75f, 100);
	}
}

void PlayerBullet::Reload(const WorldTransform& transform, bool isEmit)
{
	isReload_ = true;

	// Emitがtureならリロードのエフェクトを出す
	if (isEmit == false) { return; }
	effect_->OnceBulletReloadEffect(transform);
}

void PlayerBullet::Attack(const WorldTransform& transform, float speed)
{
	// 回転と座標を取得
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform.rotation_);
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_ + item_->GetBulletData().position.Transform(rotateMatrix);

	// 速度(向き)を取得
	speed_ = speed;
	velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix);

	activeTime_ = 0.0f;
	isReload_ = false;
	isActive_ = true;
	Collider::isActive_ = true;
	Object3d::GetMaterial().enableDraw = true;

	effect_->OnceBulletEffect(transform);
}

void PlayerBullet::IsCollision()
{
	activeTime_ = 1.0f;
	wasActive_ = false;
	isActive_ = false;
	Collider::isActive_ = false;
	Object3d::GetMaterial().enableDraw = false;
}

void PlayerBullet::SetOnDeactivateCallback(const std::function<void()>& callback)
{
	onDeactivatedCallback_ = callback;
}
