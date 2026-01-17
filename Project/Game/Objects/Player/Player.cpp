#include "Player.h"

#include "imgui.h"

#include "DeltaTimer.h"
#include "Collision/CollisionFilter.h"
#include "Objects/Player/State/PlayerMoveState.h"
#include "Objects/Player/State/PlayerDeadState.h"

void Player::Initialize()
{
	// 全ての調整項目をロード
	items_ = std::make_unique<PlayerAdjustItem>();
	items_->LoadItems();

	// シーンのオブジェクトをロード、初期化
	SceneObject player;
	for (auto it = loader_->GetData().begin(); it != loader_->GetData().end();) {
		if (it->second.tag == "Player") {
			player = it->second;
			break;
		}
		++it;
	}
	Animation::Initialize(player.fileName);
	Animation::SetSceneRenderer();
	Animation::PlayByName("Idle", 0.0f);
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = Vector3::ExprZero;
	Animation::SetTransform(player.transform);
	startTransform_ = player.transform;

	if (player.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = player.tag;
		Collider::myType_ = player.collider.type;
		Collider::offsetPosition_ = player.collider.center;
		Collider::size_ = player.collider.size;
		Collider::radius_ = player.collider.radius;
		Collider::targetColliderName_ = {
			"EnemyMelee","EnemyShieldBearer","EnemyRanged","EnemyRangedElite","EnemyRay","BossRay",
			"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall","BossEnemy","BossAttack",
		};
		Collider::DrawCollider();
	}

	// プレイヤーの初期化
	ChangeState(std::make_unique<PlayerMoveState>(this));

	// エフェクトの初期化
	effect_ = std::make_unique<PlayerEffect>();
	effect_->SetPlayer(this);
	effect_->Init();

	// レティクルの初期化
	reticle_ = std::make_unique<PlayerReticle>();
	reticle_->Init();

	// 弾の初期化
	shot_ = std::make_unique<PlayerShot>();
	shot_->Init(this);
}

void Player::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// 弾の更新
	shot_->Update();
	shot_->UpdateUI();

	// コライダーの更新
	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Animation::Update();
}

void Player::EffectDraw()
{
	// エフェクトの描画
	effect_->Draw();
}

void Player::Draw()
{	
	reticle_->Draw();

	// 弾UIのDraw処理
	shot_->DrawUI();
}

void Player::ChangeState(std::unique_ptr<PlayerBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void Player::OnCollisionEnter(Collider* other)
{
	if (CollisionFilter::CheckColliderNameEnemy(other->GetColliderName())) {
		if (!isAvoid_ && !items_->GetPlayerData().isInvincible && isAlive_) {
			isAlive_ = false;
			ChangeState(std::make_unique<PlayerDeadState>(this));
		}
	}
}

void Player::OnCollisionStay(Collider* other)
{
	// 建物系の押し出し判定(OBB,Sphere)、木の押し出し判定(OBB)
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
		isPushMove_ = true;
		Vector3 push{};
		if (other->GetMyColliderType() == ColliderType::kOBB) {
			push = Collision3D::GetOBBSpherePushVector(other, this);
		} else if (other->GetMyColliderType() == ColliderType::kSphere) {
			push = Collision3D::GetSphereSpherePushVector(other, this);
		}
		push.y = 0.0f;
		transform_.translation_ += push * items_->GetPlayerData().pushSpeed * DeltaTimer::GetDeltaTime();
		Collider::centerPosition_ = transform_.translation_;
		Collider::Update();
		Animation::TransformUpdate();
	}
}

void Player::OnCollisionExit(Collider* other)
{
	// 建物系の押し出し判定(OBB,Sphere)
	// 木の押し出し判定(OBB)
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
		isPushMove_ = false;
	}
}

void Player::Reset()
{
	// プレイヤーのリセットをする
	Animation::GetTimeStop() = false;
	Animation::PlayByName("Idle", 0.0f);
	Animation::SetTransform(startTransform_);
	ChangeState(std::make_unique<PlayerMoveState>(this));
	
	// 初期化する
	isAlive_ = true;
	isAvoid_ = false;
	avoidCoolTimer_ = 0.0f;
	isPushMove_ = false;
	isPlayingMouse_ = false;
}
