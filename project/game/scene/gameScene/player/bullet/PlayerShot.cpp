#include "PlayerShot.h"

#include "gameScene/player/Player.h"
#include "gameScene/player/adjustItem/PlayerAdjustItem.h"

void PlayerShot::Init(Player* player)
{
	player_ = player;

	// 弾を初期化
	bullets_.resize(6);
	specialBullets_.resize(6);
	bulletUIs_.resize(6);
	// 通常弾
	for (auto& bullet : bullets_) {
		bullet = std::make_unique<PlayerBullet>();
		bullet->SetItem(player->GetItem());
		bullet->Init("PlayerBullet");
	}
	// 必殺技の弾
	for (auto& bullet : specialBullets_) {
		bullet = std::make_unique<PlayerBullet>();
		bullet->SetItem(player->GetItem());
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

	// 弾の予測オブジェクトの初期化
	for (auto& object : predictionObjects_) {
		object = std::make_unique<PredictionObject>();
		object->SetPlayer(player);
		object->Init();
	}

	rightStickQuaternion_ = Quaternion::IdentityQuaternion();
}

void PlayerShot::Update()
{
	// 今リロードが終わっている弾のを取得する
	kBulletCount_ = 0;
	// キル数を保持する
	kNockdownCount_ = 0;
	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
		// 弾のリロードが終わっているならカウントに追加
		if (bullet->GetIsReload()) { ++kBulletCount_; }
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
	for (size_t i = 0; i < predictionObjects_.size(); ++i) {
		float interval = player_->GetItem()->GetPreObjectData().interval;
		Vector3 startPosition = player_->GetItem()->GetPreObjectData().startPosition;
		predictionObjects_[i]->Update(
			(Vector3::ExprUnitZ * (static_cast<float>(i) * interval) + startPosition)
		);
	}
}

void PlayerShot::UpdateUI()
{
	killCountUI_->Update(kNockdownCount_);

	size_t bulletUICount = 0;
	float startPos = player_->GetItem()->GetBulletUIData().startPosition;
	Vector2 position = player_->GetItem()->GetBulletUIData().position;
	for (auto& bulletUI : bulletUIs_) {
		bulletUI->Update(
			player_->GetItem()->GetBulletUIData().size,
			Vector2{ (bulletUICount * position.x) + startPos,position.y });
		++bulletUICount;

		if (kBulletCount_ >= bulletUICount) {
			bulletUI->GetRenderOptions().enabled = true;
		} else {
			bulletUI->GetRenderOptions().enabled = false;
		}
	}

}

void PlayerShot::DrawUI()
{
	killCountUI_->Draw();

	for (auto& bulletUI : bulletUIs_) {
		if (bulletUI->GetRenderOptions().enabled) {
			bulletUI->Draw();
		}
	}
}


void PlayerShot::ReloadBullet()
{
	// 動きが終わっている弾からリロードをする
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && !bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = player_->GetTransform().translation_;
			bullet->Reload(transform, true);
			break;
		}
	}
}

void PlayerShot::AllReloadBullet()
{
	// 動きが終わっている弾からリロードをする
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && !bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = player_->GetTransform().translation_;
			bullet->Reload(transform, true);
		}
	}
}

bool PlayerShot::IsReloadBullet()
{
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsReload()) {
			return false;
		}
	}
	return true;
}

void PlayerShot::AttackBullet()
{
	// リロードが終わっていて動いていない弾を発射する
	for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = player_->GetTransform().translation_;
			bullet->Attack(transform, player_->GetItem()->GetBulletData().speed);
			break;
		}
	}
}

void PlayerShot::SpecialAttackBullet()
{
	// 敵のTransformを取得した分だけ回す
	uint32_t count = static_cast<uint32_t>(player_->GetReticle()->GetEnemyTransforms().size() - 1);
	auto& transform = player_->GetReticle()->GetEnemyTransforms().back();

	Vector3 dir = (transform.translation_ - player_->GetTransform().translation_).Normalize();
	transform.rotation_ = Quaternion::DirectionToQuaternion(transform.rotation_, dir, 1.0f);
	transform.translation_ = player_->GetTransform().translation_;
	player_->GetTransform().rotation_ = transform.rotation_;
	specialBullets_[count]->Attack(transform, player_->GetItem()->GetBulletData().speed_sp);
	player_->GetReticle()->GetEnemyTransforms().pop_back();

	if (player_->GetReticle()->GetEnemyTransforms().empty()) {
		for (auto& bullet : specialBullets_) {
			bullet->Reload(WorldTransform());
		}
		player_->GetReticle()->GetEnemyTransforms().clear();
		player_->GetReticle()->ResetHitCount();
		AllReloadBullet();
	}
}
