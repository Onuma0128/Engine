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

	BulletInit();
}

void Player::GlobalInit()
{

}

void Player::Update()
{
	state_->Update();

	effect_->Update();

	// 弾の更新,弾UIの更新
	for (size_t i = 0; i < bulletUIs_.size(); ++i) {
		bullets_[i]->Update();
		// トレイルエフェクトを生成
		if (bullets_[i]->GetIsActive()) {
			effect_->OnceBulletTrailEffect(static_cast<int32_t>(i), bullets_[i]->GetTransform());
		}
		bulletUIs_[i]->Update({});
	}
	
	Object3d::Update();
}

void Player::Draw()
{
	state_->Draw();

	effect_->Draw();

	// 弾の描画,弾UIの描画
	for (size_t i = 0; i < bulletUIs_.size(); ++i) {
		bullets_[i]->Draw();
		if (!bullets_[i]->GetIsReload()) { continue; }
		bulletUIs_[i]->Draw();
	}
	
	Object3d::Draw();
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