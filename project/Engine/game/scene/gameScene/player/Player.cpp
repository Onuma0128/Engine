#include "Player.h"

#include "state/PlayerMoveState.h"

void Player::Init()
{
	Object3d::Initialize("Box.obj");

	transform_.scale_ = { 0.5f,0.5f,0.5f };
	transform_.translation_ = { 0.0f,0.5f,0.0f };

	ChengeState(std::make_unique<PlayerMoveState>(this));

	effect_ = std::make_unique<PlayerEffect>();
	effect_->SetPlayer(this);
	effect_->Init();

	reticle_ = std::make_unique<PlayerReticle>();
	reticle_->Init();

	BulletInit();

	Collider::AddCollider();
	Collider::colliderName_ = "Player";
	Collider::myType_ = ColliderType::OBB;
	Collider::size_ = transform_.scale_;
	Collider::DrawCollider();
}

void Player::GlobalInit()
{

}

void Player::Update()
{
	state_->Update();

	effect_->Update();

	// 今リロードが終わっている弾のを取得する
	size_t bulletCount = 0;
	// 弾の更新,弾UIの更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// トレイルエフェクトを生成
		if (bullet->GetIsActive()) {
			effect_->OnceBulletTrailEffect(static_cast<int32_t>(bulletCount), bullet->GetTransform());
		}
		// 弾が当たった時のエフェクト
		if (bullet->GetIsCollision()) {
			bullet->SetIsCollision(false);
			effect_->OnceBulletDeleteEffect(static_cast<int32_t>(bulletCount), bullet->GetTransform());
		}
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([this, bulletCount, &bullet]() {
			this->GetEffect()->OnceBulletDeleteEffect(static_cast<int32_t>(bulletCount), bullet->GetTransform());
		});
		// 弾のリロードが終わっているならカウントに追加
		if (bullet->GetIsReload()) { ++bulletCount; }
	}

	size_t bulletUICount = 0;
	for (auto& bulletUI : bulletUIs_) {
		bulletUI->Update({});
		++bulletUICount;

		if (bulletCount >= bulletUICount) {
			bulletUI->GetRenderOptions().enabled = true;
		} else {
			bulletUI->GetRenderOptions().enabled = false;
		}
	}

	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}

void Player::ChengeState(std::unique_ptr<PlayerBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void Player::ReloadBullet()
{
	// 動きが終わっている弾からリロードをする
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && !bullet->GetIsReload()) {
			bullet->Reload();
			break;
		}
	}
}

void Player::AttackBullet()
{
	// リロードが終わっていて動いていない弾を発射する
	for (int32_t i = static_cast<int32_t>(bullets_.size() - 1); i >= 0; --i) {
		if (!bullets_[i]->GetIsActive() && bullets_[i]->GetIsReload()) {
			bullets_[i]->Attack(transform_);
			effect_->OnceBulletEffect();
			break;
		}
	}
}

void Player::BulletInit()
{
	// 弾を初期化
	bullets_.resize(6);
	bulletUIs_.resize(6);
	for (auto& bullet : bullets_) {
		bullet = std::make_unique<PlayerBullet>();
		bullet->Init();
	}
	// 弾UIを初期化
	for (size_t i = 0; i < bulletUIs_.size(); ++i) {
		bulletUIs_[i] = std::make_unique<PlayerBulletUI>();
		bulletUIs_[i]->Init(Vector2{ (i * 32.0f) + 32.0f,32.0f });
	}
}