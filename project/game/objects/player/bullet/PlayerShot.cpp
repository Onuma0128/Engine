#include "PlayerShot.h"

#include "Input.h"

#include "objects/player/Player.h"
#include "objects/player/adjustItem/PlayerAdjustItem.h"

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
	killCountUI_ = std::make_unique<PlayerCountUI>();
	killCountUI_->Init();

	// 弾の予測オブジェクトの初期化
	for (auto& object : predictionObjects_) {
		object = std::make_unique<PredictionObject>();
		object->SetPlayer(player);
		object->Init();
	}

	rightStickQuaternion_ = Quaternion::IdentityQuaternion();

	// プレイヤーの目線のコライダーを初期化
	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "PlayerShotRay";
	Collider::isActive_ = true;
	Collider::targetColliderName_ = { "Enemy" };
	Collider::DrawCollider();

	rayReticle_ = std::make_unique<PlayerRayReticle>();
	rayReticle_->Init();
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

	RayUpdate();
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

	rayReticle_->Draw();

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
			Input::GetInstance()->Vibrate(0.3f, 0.6f, 60);
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
			Input::GetInstance()->Vibrate(0.5f, 0.9f, 90);
			++kHitRate_;
			break;
		}
	}
}

void PlayerShot::SpecialAttackBullet()
{
	// 敵のTransformを取得した分だけ回す
	uint32_t count = static_cast<uint32_t>(player_->GetReticle()->GetEnemyColliders().size() - 1);
	auto& collider = player_->GetReticle()->GetEnemyColliders().back();
	WorldTransform transform;
	transform.rotation_ = collider->GetRotate();
	transform.translation_ = collider->GetCenterPosition();

	Vector3 dir = (transform.translation_ - player_->GetTransform().translation_).Normalize();
	transform.rotation_ = Quaternion::DirectionToQuaternion(transform.rotation_, dir, 1.0f);
	transform.translation_ = player_->GetTransform().translation_;
	player_->GetTransform().rotation_ = transform.rotation_;
	specialBullets_[count]->Attack(transform, player_->GetItem()->GetBulletData().speed_sp);
	player_->GetReticle()->EnemyCollidersPopBack();
	++kHitRate_;

	if (player_->GetReticle()->GetEnemyColliders().empty()) {
		for (auto& bullet : specialBullets_) {
			bullet->Reload(WorldTransform());
		}
		player_->GetReticle()->EnemyCollidersClear();
		player_->GetReticle()->ResetHitCount();
		AllReloadBullet();
	}
}

void PlayerShot::OnCollisionStay(Collider* other)
{
	isRayHit_ = true;

	if (rayHitPosition_ == Vector3::ExprZero) {
		rayHitPosition_ = other->GetCenterPosition();
	} else {
		float distanceNow = Vector3::Distance(player_->GetTransform().translation_, rayHitPosition_);
		float distanceOther = Vector3::Distance(player_->GetTransform().translation_, other->GetCenterPosition());
		if (distanceOther < distanceNow) {
			rayHitPosition_ = other->GetCenterPosition();
		}
	}
}

void PlayerShot::RayUpdate()
{
	Vector3 rotateVelocity{};
	Input::GetInstance()->SetGamepadStickDeadzoneScale(0.2f);
	rotateVelocity.x = Input::GetInstance()->GetGamepadRightStickX();
	rotateVelocity.z = Input::GetInstance()->GetGamepadRightStickY();
	Input::GetInstance()->SetGamepadStickDeadzoneScale(1.0f);
	Quaternion rightQuaternion = Quaternion::IdentityQuaternion();
	if (rotateVelocity.Length() < 0.01f) {
		rightQuaternion = player_->GetTransform().rotation_;
		Collider::isActive_ = false;
	} else {
		rightQuaternion = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rotateVelocity, 1.0f);
		Collider::isActive_ = true;
	}
	const auto& itemData = player_->GetItem()->GetPreObjectData();
	const auto rotateMatrix = Quaternion::MakeRotateMatrix(rightQuaternion);
	auto centerPosition = player_->GetTransform().translation_ + itemData.rayColliderPosition.Transform(rotateMatrix);
	
	Collider::origin_ = centerPosition;
	Collider::diff_ = (Vector3::ExprUnitZ * itemData.rayColliderSize.z).Transform(rotateMatrix);

	Collider::size_ = itemData.rayColliderSize;
	Collider::rotate_ = rightQuaternion;
	Collider::centerPosition_ = centerPosition;
	Collider::Update();

	rayReticle_->SetRaticleAlpha(isRayHit_);
	rayReticle_->SetPosition(rayHitPosition_);
	rayReticle_->Update();

	ResetRayHit();
}
