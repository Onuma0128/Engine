#include "Enemy.h"

#include "gameScene/enemy/state/EnemyMoveState.h"
#include "gameScene/gameCamera/GameCamera.h"

void Enemy::Init()
{
	Object3d::Initialize("Box.obj");

	transform_.scale_ = { 0.5f,0.5f,0.5f };
	transform_.translation_ = { 0.0f,0.5f,0.0f };

	ChengeState(std::make_unique<EnemyMoveState>(this));

	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "Enemy";
	Collider::size_ = transform_.scale_;
	Collider::DrawCollider();
}

void Enemy::Update()
{
	state_->Update();

	Collider::centerPosition_ = transform_.translation_;
	Collider::rotate_ = transform_.rotation_;
	Collider::Update();

	Object3d::Update();
}

void Enemy::ChengeState(std::unique_ptr<EnemyBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void Enemy::OnCollisionEnter(Collider* other)
{
	if (other->GetColliderName() == "PlayerBullet") {
		WorldTransform bulletTrans;
		bulletTrans.rotation_ = other->GetRotate();
		bulletTrans.translation_ = other->GetCenterPosition();
		effect_->OnceBulletHitEffect(bulletTrans);
		gameCamera_->SetShake(1.0f);
	}
}
