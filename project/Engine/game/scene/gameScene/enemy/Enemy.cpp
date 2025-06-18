#include "Enemy.h"

#include "DeltaTimer.h"

#include "gameScene/enemy/state/EnemyMoveState.h"
#include "gameScene/enemy/state/EnemyDeadState.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void Enemy::Finalize()
{
	Object3d::RemoveRenderer();
	Collider::RemoveCollider();

	for (auto& bullet : bullets_) {
		bullet->Finalize();
	}
}

void Enemy::Init()
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::SetColor(Vector4{ 1.0f,0.0f,0.0f,1.0f });

	transform_.scale_ = { 0.5f,0.75f,0.5f };
	transform_.translation_ = { 0.0f,0.75f,0.0f };

	ChengeState(std::make_unique<EnemyMoveState>(this));

	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "Enemy";
	Collider::size_ = transform_.scale_;
	Collider::DrawCollider();

	int type = rand() % 2;
	type_ = static_cast<EnemyType>(type);

	BulletInit();
}

void Enemy::Update()
{
	state_->Update();

	effect_->Update();

	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
	}

	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::Shrinking) {
		if (isAlive_) {
			Collider::isActive_ = true;
			hitReticle_ = false;
		}
	} else if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::None) {
		Object3d::GetRenderOptions().offscreen = true;
	}

	Collider::centerPosition_ = transform_.translation_;
	Collider::rotate_ = transform_.rotation_;
	Collider::Update();

	Object3d::Update();
}

void Enemy::Draw()
{
	effect_->Draw();
}

void Enemy::ChengeState(std::unique_ptr<EnemyBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void Enemy::Reset(const Vector3& position)
{
	transform_.scale_ = { 0.5f,0.75f,0.5f };
	transform_.rotation_ = Quaternion::IdentityQuaternion();
	transform_.translation_ = position;

	ChengeState(std::make_unique<EnemyMoveState>(this));
	Collider::isActive_ = true;
	isAlive_ = true;
	isDead_ = false;
	hitReticle_ = false;
}

void Enemy::OnCollisionEnter(Collider* other)
{
	// プレイヤーの弾と当たっているなら
	if (other->GetColliderName() == "PlayerBullet") {
		gameCamera_->SetShake(2.0f);
	}
	if (other->GetColliderName() == "PlayerBulletSpecial") {
		gameCamera_->SetShake(5.0f);
		DeltaTimer::SetTimeScaleForSeconds(0.1f, 0.1f);
	}
	if (other->GetColliderName() == "PlayerBullet" || other->GetColliderName() == "PlayerBulletSpecial") {
		Collider::isActive_ = false;
		isAlive_ = false;
		// 敵がノックバックする方向を取得
		Matrix4x4 rotate = Quaternion::MakeRotateMatrix(other->GetRotate());
		velocity_ = Vector3::ExprUnitZ.Transform(rotate);
		playerBulletPosition_ = other->GetCenterPosition();
		// エフェクトを描画
		WorldTransform transform;
		transform.rotation_ = other->GetRotate();
		transform.translation_ = transform_.translation_;
		effect_->OnceBulletHitEffect(transform);
		transform.rotation_ = transform_.rotation_;
		transform.translation_ = transform_.translation_ - (velocity_ * 0.5f);
		effect_->OnceBulletHitExplosionEffect(transform);
		// 死亡時のステートに遷移
		ChengeState(std::make_unique<EnemyDeadState>(this));
	}

	// プレイヤーのレティクルと当たっているなら
	if (other->GetColliderName() == "PlayerReticle") {
		hitReticle_ = true;
		Object3d::GetRenderOptions().offscreen = false;
		Collider::isActive_ = false;
	}
}

void Enemy::OnCollisionStay(Collider* other)
{
	// プレイヤーと当たっているなら
	if (other->GetColliderName() == "Player") {
		const float speed = 2.0f;
		transform_.translation_ -= velocity_ * speed * DeltaTimer::GetDeltaTime();
		Object3d::Update();
	}

	// 敵と当たっているなら
	if (other->GetColliderName() == "Enemy") {
		const float speed = 1.0f;
		Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
		transform_.translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
		Object3d::Update();
	}

	//// プレイヤーのレティクルと当たっているなら
	//if (other->GetColliderName() == "PlayerReticle") {
	//	hitReticle_ = true;
	//	Object3d::GetRenderOptions().offscreen = false;
	//	Collider::isActive_ = false;
	//}
}

void Enemy::OnCollisionExit(Collider* other)
{
	// プレイヤーと当たっているなら
	// 敵と当たっているなら
	if (other->GetColliderName() == "Player" ||
		other->GetColliderName() == "Enemy") {
	}

	if (other->GetColliderName() == "PlayerReticle") {
		//Object3d::GetRenderOptions().offscreen = true;
	}
}

void Enemy::BulletInit()
{
	switch (type_)
	{
	case EnemyType::Ranged:
	{
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->SetItem(items_);
		bullet->Init("EnemyRanged", type_);
		bullets_.push_back(std::move(bullet));
	}
		break;
	case EnemyType::RangedElite:
	{
		for (uint32_t i = 0; i < 3; ++i) {
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			bullet->SetItem(items_);
			bullet->Init("EnemyRangedElite", type_);
			bullets_.push_back(std::move(bullet));
		}
	}
		break;
	default:	break;
	}
}