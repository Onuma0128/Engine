#include "BaseEnemy.h"

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

#include "../state/EnemyMoveState.h"
#include "../state/EnemyDeadState.h"

void BaseEnemy::Initialize()
{
	// ステートを初期化
	ChengeState(std::make_unique<EnemyMoveState>(this));

	// エフェクトの初期化
	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	// 影を設定
	shadow_ = std::make_unique<EnemyShadow>();
	shadow_->Init(transform_);
	shadow_->SetDraw(false);

	// 探索アルゴリズムの初期化
	pathFinder_.Search(Animation::transform_.translation_, Vector3{});

	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "Enemy";
	Collider::size_ = transform_.scale_;
	Collider::isActive_ = false;
	Collider::DrawCollider();

	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = { 0.0f,0.0f,0.0f };
	outlineColor_ = { 0.0f,0.0f,0.0f };
}

void BaseEnemy::Update()
{
	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// 必殺技が打たれている時の敵の描画を変更
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::Shrinking) {
		if (stateParam_.isAlive_) {
			Collider::isActive_ = true;
			stateParam_.hitReticle_ = false;
		}
	}
	if (player_->GetEffect()->GetSpecialState() == SpecialMoveState::Holding) {
		if (!stateParam_.hitReticle_) {
			outlineColor_ = Vector3::Lerp(outlineColor_, Vector3::ExprUnitX, 0.1f);
		} else {
			outlineColor_ = Vector3::Lerp(outlineColor_, Vector3{1.0f,1.0f,0.0f}, 0.1f);
		}
	} else {
		outlineColor_ = Vector3::Lerp(outlineColor_, Vector3::ExprZero, 0.1f);
	}

	// 影の更新
	shadow_->Update();

	// 敵コライダーの更新
	Collider::size_ = items_->GetMainData().colliderSize;
	Collider::rotate_ = transform_.rotation_;
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
		// 影の更新
		shadow_->Update();
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
	Animation::GetMaterial().enableDraw = false;
	Animation::GetTimeStop() = true;
	Collider::isActive_ = false;
	// 影の描画も切る
	shadow_->SetDraw(false);
	stateParam_.enableMove_ = false;
}

void BaseEnemy::Reset(const Vector3& position)
{
	// 描画をする
	Animation::GetMaterial().enableDraw = true;
	// Transformを初期化
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotation_ = Quaternion::IdentityQuaternion();
	transform_.translation_ = position;
	Animation::TransformUpdate();
	// 影を描画する
	shadow_->SetDraw(true);
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

void BaseEnemy::OnCollisionExit(Collider* other)
{
}