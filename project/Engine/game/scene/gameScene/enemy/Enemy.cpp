#include "Enemy.h"

#include "DeltaTimer.h"

#include "gameScene/enemy/state/EnemyMoveState.h"
#include "gameScene/enemy/state/EnemyDeadState.h"
#include "gameScene/enemy/weapon/axe/EnemyAxe.h"
#include "gameScene/enemy/weapon/shield/EnemyShield.h"

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
	int type = rand() % 4;
	type_ = static_cast<EnemyType>(type);

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

	EnemyTypeInit();
}

void Enemy::Update()
{
	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// ウエポンの更新
	if (enemyWeapon_ != nullptr) { enemyWeapon_->Update(); }

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
	}

	// 必殺技が打たれている時の敵の描画を変更
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::Shrinking) {
		if (isAlive_) {
			Collider::isActive_ = true;
			hitReticle_ = false;
		}
	} else if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::None) {
		Object3d::GetRenderOptions().offscreen = true;
	}

	// 敵コライダーの更新
	Collider::centerPosition_ = transform_.translation_;
	Collider::rotate_ = transform_.rotation_;
	Collider::Update();

	// オブジェクトの更新
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
}

void Enemy::OnCollisionExit(Collider* other)
{
	// プレイヤーと当たっているなら
	// 敵と当たっているなら
	if (other->GetColliderName() == "Player" ||
		other->GetColliderName() == "Enemy") {
	}
}

void Enemy::EnemyTypeInit()
{
	// タイプごとに作成する
	switch (type_)
	{
	case EnemyType::Melee:
	{
		// 近接攻撃用のコライダーを作成
		enemyWeapon_ = std::make_unique<EnemyAxe>(this);
		enemyWeapon_->Init(ColliderType::Sphere, "EnemyMelee");
	}
	break;
	case EnemyType::Ranged:
	{
		// 弾を1つ作成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->SetItem(items_);
		bullet->Init("EnemyRanged", type_);
		bullets_.push_back(std::move(bullet));
	}
	break;
	case EnemyType::ShieldBearer:
	{
		// シールド用のコライダーを作成
		enemyWeapon_ = std::make_unique<EnemyShield>(this);
		enemyWeapon_->Init(ColliderType::OBB, "EnemyShieldBearer");
	}
	break;
	case EnemyType::RangedElite:
	{
		// 弾を3つ作成
		for (uint32_t i = 0; i < 3; ++i) {
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			bullet->SetItem(items_);
			bullet->Init("EnemyRangedElite", type_);
			bullets_.push_back(std::move(bullet));
		}
	}
	break;
	default:
		break;
	}
}
