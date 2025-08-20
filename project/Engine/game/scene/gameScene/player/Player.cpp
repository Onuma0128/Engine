#include "Player.h"

#include "imgui.h"

#include "DeltaTimer.h"
#include "state/PlayerMoveState.h"
#include "state/PlayerDeadState.h"

void Player::Init(SceneJsonLoader loader)
{
	// 全ての調整項目をロード
	items_ = std::make_unique<PlayerAdjustItem>();
	items_->LoadItems();

	// シーンのオブジェクトをロード、初期化
	SceneObject player;
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.tag == "Player") {
			player = it->second;
			break;
		}
		++it;
	}
	Animation::Initialize(player.fileName);
	Animation::SetSceneRenderer();
	Animation::PlayByName("Idle_Gun", 0.0f);
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = { 0.0f,0.0f,0.0f };

	transform_ = player.transform;
	if (player.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = "Player";
		Collider::myType_ = player.collider.type;
		Collider::offsetPosition_ = player.collider.center;
		Collider::size_ = player.collider.size;
		Collider::radius_ = player.collider.radius;
		Collider::DrawCollider();
	}

	// プレイヤーの初期化
	ChengeState(std::make_unique<PlayerMoveState>(this));

	effect_ = std::make_unique<PlayerEffect>();
	effect_->SetPlayer(this);
	effect_->Init();

	reticle_ = std::make_unique<PlayerReticle>();
	reticle_->Init();

	shot_ = std::make_unique<PlayerShot>();
	shot_->Init(this);
}

void Player::Update()
{
#ifdef _DEBUG
	items_->Editor();
#endif // _DEBUG

	state_->Update();

	effect_->Update();

	// 弾の更新
	shot_->Update();
	shot_->UpdateUI();

	transform_.translation_.x = std::clamp(transform_.translation_.x, -50.0f, 50.0f);
	transform_.translation_.z = std::clamp(transform_.translation_.z, -50.0f, 50.0f);
	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Animation::Update();
}

void Player::EffectDraw()
{
	effect_->Draw();
}

void Player::Draw()
{	
	reticle_->Draw();

	// 弾UIのDraw処理
	shot_->DrawUI();
}

void Player::ChengeState(std::unique_ptr<PlayerBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void Player::OnCollisionEnter(Collider* other)
{
	if (other->GetColliderName() == "EnemyMelee" ||
		other->GetColliderName() == "EnemyRanged" ||
		other->GetColliderName() == "EnemyShieldBearer" ||
		other->GetColliderName() == "EnemyRangedElite") {
		if (!isAvoid_ && !items_->GetPlayerData().isInvincible) {
			//isAlive_ = false;
			ChengeState(std::make_unique<PlayerDeadState>(this));
		}
	}
}

void Player::OnCollisionStay(Collider* other)
{
	// 建物系の押し出し判定(OBB,Sphere)、木の押し出し判定(OBB)
	if (other->GetColliderName() == "Building" || 
		other->GetColliderName() == "DeadTree" ||
		other->GetColliderName() == "fence") {
		isPushMove_ = true;
		Vector3 push{};
		if (other->GetMyColliderType() == ColliderType::OBB) {
			push = Collision3D::GetOBBSpherePushVector(other, this);
		} else if (other->GetMyColliderType() == ColliderType::Sphere) {
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
	if (other->GetColliderName() == "Building" || 
		other->GetColliderName() == "DeadTree" ||
		other->GetColliderName() == "fence") {
		isPushMove_ = false;
	}
}
