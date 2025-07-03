#include "Enemy.h"

#include "DeltaTimer.h"

#include "gameScene/enemy/state/EnemyMoveState.h"
#include "gameScene/enemy/state/EnemyDeadState.h"
#include "gameScene/enemy/weapon/axe/EnemyAxe.h"
#include "gameScene/enemy/weapon/shield/EnemyShield.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

void Enemy::Finalize()
{
	Animation::RemoveRenderer();
	Collider::RemoveCollider();

	for (auto& bullet : bullets_) {
		bullet->Finalize();
	}

	if (weapon_ != nullptr) { weapon_->Finalize(); }
	if (shieldWeapon_ != nullptr) { shieldWeapon_->Finalize(); }
}

void Enemy::Init(EnemyType type)
{
	// タイプごとに敵を初期化する
	type_ = type;
	TypeInit();

	// 最初は移動ステート開始
	ChengeState(std::make_unique<EnemyMoveState>(this));

	// エフェクトの初期化
	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	// 影を設定
	shadow_ = std::make_unique<EnemyShadow>();
	shadow_->Init(transform_);
	shadow_->SetDraw(false);

	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "Enemy";
	Collider::size_ = transform_.scale_;
	Collider::isActive_ = false;
	Collider::DrawCollider();
}

void Enemy::Update()
{
	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// ウエポンの更新
	if (weapon_ != nullptr) { weapon_->Update(); }
	if (shieldWeapon_ != nullptr) { shieldWeapon_->Update(); }

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
	}

	// 必殺技が打たれている時の敵の描画を変更
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::Shrinking) {
		if (stateParam_.isAlive_) {
			Collider::isActive_ = true;
			stateParam_.hitReticle_ = false;
		}
	} else if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::None) {
		Animation::GetRenderOptions().offscreen = true;
	}

	// 影の更新
	shadow_->Update();

	// 敵コライダーの更新
	Collider::size_ = items_->GetMainData().colliderSize;
	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	Collider::Update();

	// オブジェクトの更新
	Animation::Update();
}

void Enemy::TransformUpdate() 
{
	// 敵の行動許可が出ていなければ更新できない
	if (!stateParam_.enableMove_) {
		// ウエポンの更新
		if (shieldWeapon_ != nullptr) { shieldWeapon_->Update(); }
		// 影の更新
		shadow_->Update();
		// オブジェクトの更新
		Animation::TransformUpdate();
	}
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

void Enemy::Dead()
{
	Animation::GetRenderOptions().enabled = false;
	Animation::GetTimeStop() = true;
	Collider::isActive_ = false;

	if (shieldWeapon_ != nullptr) { shieldWeapon_->SetIsActive(false); }
	shadow_->SetDraw(false);

	stateParam_.enableMove_ = false;
}

void Enemy::Reset(const Vector3& position)
{
	if (type_ == EnemyType::Melee || type_ == EnemyType::ShieldBearer) {
		Animation::PlayByName("Run_Arms");
	} else {
		Animation::PlayByName("Run");
	}
	Animation::GetRenderOptions().enabled = true;
	Animation::GetRenderOptions().offscreen = true;

	transform_.scale_ = { 1.5f,1.5f,1.5f };
	transform_.rotation_ = Quaternion::IdentityQuaternion();
	transform_.translation_ = position;
	Animation::TransformUpdate();

	ChengeState(std::make_unique<EnemyMoveState>(this));

	if (shieldWeapon_ != nullptr) { shieldWeapon_->SetIsActive(true); }
	shadow_->SetDraw(true);

	Collider::isActive_ = true;
	stateParam_.isAlive_ = true;
	stateParam_.isDead_ = false;
	stateParam_.hitReticle_ = false;
	stateParam_.enableMove_ = true;
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
		stateParam_.isAlive_ = false;
		if (shieldWeapon_ != nullptr) { shieldWeapon_->SetIsActive(false); }
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
		stateParam_.hitReticle_ = true;
		Animation::GetRenderOptions().offscreen = false;
		Collider::isActive_ = false;
	}
}

void Enemy::OnCollisionStay(Collider* other)
{
	// プレイヤーと当たっているなら
	if (other->GetColliderName() == "Player") {
		const float speed = 2.0f;
		transform_.translation_ -= velocity_ * speed * DeltaTimer::GetDeltaTime();
		Animation::TransformUpdate();
	}

	// 敵と当たっているなら
	if (other->GetColliderName() == "Enemy") {
		const float speed = 1.0f;
		Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
		transform_.translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
		Animation::TransformUpdate();
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

void Enemy::TypeInit()
{
	// タイプごとに作成する
	switch (type_)
	{
	case EnemyType::Melee:
	{
		// 敵Animationの初期化
		Animation::Initialize("Zombie_Basic.gltf");
		Animation::PlayByName("Idle");
		// 近接攻撃用のコライダーを作成
		weapon_ = std::make_unique<EnemyAxe>(this);
		weapon_->Init(ColliderType::Sphere, "EnemyMelee");
	}
	break;
	case EnemyType::Ranged:
	{
		// 敵Animationの初期化
		Animation::Initialize("Characters_Shaun.gltf");
		Animation::PlayByName("Idle");
		// 弾を1つ作成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->SetItem(items_);
		bullet->Init("EnemyRanged", type_);
		bullets_.push_back(std::move(bullet));
	}
	break;
	case EnemyType::ShieldBearer:
	{
		// 敵Animationの初期化
		Animation::Initialize("Zombie_Basic.gltf");
		Animation::PlayByName("Idle");
		// シールド用のコライダーを作成
		shieldWeapon_ = std::make_unique<EnemyShield>(this);
		shieldWeapon_->Init(ColliderType::OBB, "EnemyShield");
		shieldWeapon_->SetIsActive(false);
		// 攻撃コライダーを作成
		weapon_ = std::make_unique<EnemyAxe>(this);
		weapon_->Init(ColliderType::Sphere, "EnemyShieldBearer");
	}
	break;
	case EnemyType::RangedElite:
	{
		// 敵Animationの初期化
		Animation::Initialize("Characters_Shaun.gltf");
		Animation::PlayByName("Idle");
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

	Animation::SetSceneRenderer();
	Animation::GetRenderOptions().enabled = false;
	Animation::SetDrawBone(false);
	Animation::GetTimeStop() = true;
	transform_.scale_ *= 1.5f;
}
