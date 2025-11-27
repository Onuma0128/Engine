#include "BaseEnemy.h"

#include "DeltaTimer.h"

#include "objects/player/Player.h"
#include "gameCamera/GameCamera.h"
#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

#include "objects/enemy/state/EnemyMoveState.h"
#include "objects/enemy/state/EnemyDeadState.h"

void BaseEnemy::Initialize()
{
	// 敵の視線を設定
	ray_ = std::make_unique<EnemyRay>();
	ray_->Init();

	// ステートを初期化
	ChengeState(std::make_unique<EnemyMoveState>(this));

	// エフェクトの初期化
	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	// 探索アルゴリズムの初期化
	pathFinder_.Search(Animation::transform_.translation_, Vector3{});

	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSphere;
	Collider::colliderName_ = "Enemy";
	Collider::size_ = transform_.scale_;
	Collider::radius_ = transform_.scale_.x;
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Player","PlayerBullet","PlayerBulletSpecial",
		"PlayerReticle","Enemy" ,"PlayerShotRay"
	};
	Collider::DrawCollider();

	// 基本的なアニメーションを設定
	Animation::GetTransform().translation_ = items_->GetMainData().startPosition;
	Animation::SetSceneRenderer();
	Animation::GetMaterial().enableDraw = false;
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = Vector3::ExprZero;
	Animation::GetMaterial().shadowMap = false;
	Animation::GetTimeStop() = true;
	outlineColor_ = Vector3::ExprZero;
}

void BaseEnemy::Update()
{
	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// 必殺技が打たれている時の敵の描画を変更
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::kShrinking) {
		if (stateParam_.isAlive_) {
			Collider::isActive_ = true;
			stateParam_.hitReticle_ = false;
		}
	}
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::kHolding && stateParam_.isAlive_) {
		if (!stateParam_.hitReticle_) {
			outlineColor_ = Vector3::Lerp(outlineColor_, Vector3::ExprUnitX, 0.1f);
		} else {
			Vector3 color = Vector3::ExprUnitX + Vector3::ExprUnitY;
			outlineColor_ = Vector3::Lerp(outlineColor_, color, 0.1f);
		}
	} else {
		outlineColor_ = Vector3::Lerp(outlineColor_, Vector3::ExprZero, 0.1f);
	}

	// 敵コライダーの更新
	Collider::radius_ = items_->GetMainData().colliderSize;
	Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	Collider::Update();

	// アニメーションの更新
	Animation::GetMaterial().outlineColor = outlineColor_;
	Animation::Update();
}

void BaseEnemy::TransformUpdate()
{
	// 敵の行動許可が出ていなければ更新できない
	if (!stateParam_.enableMove_) {
		// オブジェクトの更新
		Animation::TransformUpdate();
	}
}

void BaseEnemy::ChengeState(std::unique_ptr<EnemyBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void BaseEnemy::Dead()
{
	// 描画とColliderを切る
	Animation::GetTransform().translation_ = items_->GetMainData().startPosition;
	Animation::GetMaterial().enableDraw = false;
	Animation::GetTimeStop() = true;
	Collider::isActive_ = false;
	// 影の描画も切る
	Animation::GetMaterial().shadowMap = false;
	stateParam_.enableMove_ = false;
}

void BaseEnemy::Reset(const Vector3& position)
{
	// 描画をする
	Animation::GetMaterial().enableDraw = true;
	// Transformを初期化
	transform_.scale_ = Vector3::ExprUnitXYZ;
	transform_.rotation_ = Quaternion::IdentityQuaternion();
	transform_.translation_ = position;
	Animation::TransformUpdate();
	// 影を描画する
	Animation::GetMaterial().shadowMap = true;
	// ColliderをActiveに戻す
	Collider::isActive_ = true;
	stateParam_.isAlive_ = true;
	stateParam_.isDead_ = false;
	stateParam_.hitReticle_ = false;
	stateParam_.enableMove_ = true;
}

void BaseEnemy::ResetSearch()
{
	pathFinder_.Search(Animation::transform_.translation_, player_->GetTransform().translation_);
}

void BaseEnemy::OnCollisionEnter(Collider* other)
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
		Collider::isActive_ = false;
	}
}

void BaseEnemy::OnCollisionStay(Collider* other)
{
	// プレイヤーと当たっているなら
	if (other->GetColliderName() == "Player") {
		const float speed = 2.0f;
		transform_.translation_ -= velocity_ * speed * DeltaTimer::GetDeltaTime();
	}

	// 敵と当たっているなら
	if (other->GetColliderName() == "Enemy") {
		const float speed = 1.0f;
		Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
		transform_.translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
	}

	if (other->GetColliderName() == "PlayerShotRay") {
		outlineColor_ = Vector3::ExprUnitX;
	}
}

void BaseEnemy::OnCollisionExit(Collider* other)
{
}