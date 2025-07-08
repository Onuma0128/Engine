#include "Player.h"

#include "imgui.h"

#include "state/PlayerMoveState.h"

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
	Animation::PlayByName("Idle_Gun");
	transform_ = player.transform;
	if (player.collider.active) {
		Collider::AddCollider();
		Collider::colliderName_ = "Player";
		Collider::myType_ = player.collider.type;
		Collider::offsetPosition_ = player.collider.center;
		Collider::size_ = player.collider.size;
		Collider::DrawCollider();
	}

	// プレイヤーの初期化
	ChengeState(std::make_unique<PlayerMoveState>(this));

	effect_ = std::make_unique<PlayerEffect>();
	effect_->SetPlayer(this);
	effect_->Init();

	reticle_ = std::make_unique<PlayerReticle>();
	reticle_->Init();

	BulletInit();
	PredictionObjInit();

	rightStickQuaternion_ = Quaternion::IdentityQuaternion();
}

void Player::GlobalInit()
{

}

void Player::Update()
{
#ifdef _DEBUG
	items_->Editor();
#endif // _DEBUG

	state_->Update();

	effect_->Update();

	// 今リロードが終わっている弾のを取得する
	size_t bulletCount = 0;
	// キル数を保持する
	kNockdownCount_ = 0;
	// 弾の更新,弾UIの更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
		// 弾のリロードが終わっているならカウントに追加
		if (bullet->GetIsReload()) { ++bulletCount; }
		// キル数を取得
		kNockdownCount_ += bullet->GetNockdownCount();
	}
	for (auto& bullet : specialBullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
		// キル数を取得
		kNockdownCount_ += bullet->GetNockdownCount();
	}

	killCountUI_->Update(kNockdownCount_);

	size_t bulletUICount = 0;
	for (auto& bulletUI : bulletUIs_) {
		bulletUI->Update(
			items_->GetBulletUIData().size,
			Vector2{ (bulletUICount * items_->GetBulletUIData().position.x) + items_->GetBulletUIData().startPosition,items_->GetBulletUIData().position.y });
		++bulletUICount;

		if (bulletCount >= bulletUICount) {
			bulletUI->GetRenderOptions().enabled = true;
		} else {
			bulletUI->GetRenderOptions().enabled = false;
		}
	}

	for (size_t i = 0; i < predictionObjects_.size(); ++i) {
		float interval = items_->GetPreObjectData().interval;
		Vector3 startPosition = items_->GetPreObjectData().startPosition;
		predictionObjects_[i]->Update(
			(Vector3::ExprUnitZ * (static_cast<float>(i) * interval) + startPosition)
		);
	}

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

	killCountUI_->Draw();

	for (auto& bulletUI : bulletUIs_) {
		if (bulletUI->GetRenderOptions().enabled) {
			bulletUI->Draw();
		}
	}
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
		if (!isAvoid_) {
			isAlive_ = false;
		}
	}
}

void Player::OnCollisionStay(Collider* other)
{
}

void Player::OnCollisionExit(Collider* other)
{
}

void Player::ReloadBullet()
{
	// 動きが終わっている弾からリロードをする
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && !bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = transform_.translation_;
			bullet->Reload(transform, true);
			break;
		}
	}
}

void Player::AttackBullet()
{
	// リロードが終わっていて動いていない弾を発射する
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = transform_.translation_;
			bullet->Attack(transform, items_->GetBulletData().speed);
			break;
		}
	}
}

void Player::SpecialAttackBullet()
{
	if (reticle_->GetEnemyTransforms().empty()) { return; }
	for (auto& bullet : specialBullets_) {
		bullet->Reload(WorldTransform());
	}
	// 敵のTransformを取得した分だけ回す
	uint32_t count = 0;
	for (auto& transform : reticle_->GetEnemyTransforms()) {
		Vector3 dir = (transform.translation_ - transform_.translation_).Normalize();
		transform.rotation_ = Quaternion::DirectionToQuaternion(transform.rotation_, dir, 1.0f);
		transform.translation_ = transform_.translation_;
		specialBullets_[count]->Attack(transform, items_->GetBulletData().speed_sp);
		++count;
	}
	reticle_->GetEnemyTransforms().clear();
	reticle_->ResetHitCount();
}

void Player::BulletInit()
{
	// 弾を初期化
	bullets_.resize(6);
	specialBullets_.resize(6);
	bulletUIs_.resize(6);
	for (auto& bullet : bullets_) {
		bullet = std::make_unique<PlayerBullet>();
		bullet->SetItem(items_.get());
		bullet->Init("PlayerBullet");
	}
	for (auto& bullet : specialBullets_) {
		bullet = std::make_unique<PlayerBullet>();
		bullet->SetItem(items_.get());
		bullet->Init("PlayerBulletSpecial");
	}
	// 弾UIを初期化
	for (size_t i = 0; i < bulletUIs_.size(); ++i) {
		bulletUIs_[i] = std::make_unique<PlayerBulletUI>();
		bulletUIs_[i]->Init(Vector2{ (i * 32.0f) + 32.0f,32.0f });
	}
	// Kill数UIの初期化
	killCountUI_ = std::make_unique<PlayerKillCountUI>();
	killCountUI_->Init();
}

void Player::PredictionObjInit()
{
	// 弾の予測オブジェクトの初期化
	for (auto& object : predictionObjects_) {
		object = std::make_unique<PredictionObject>();
		object->SetPlayer(this);
		object->Init();
	}
}
