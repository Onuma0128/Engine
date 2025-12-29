#include "PlayerShot.h"

#include "Input.h"

#include "objects/player/Player.h"
#include "objects/player/adjustItem/PlayerAdjustItem.h"

void PlayerShot::Init(Player* player)
{
	player_ = player;

	//// 弾を初期化
	//const size_t kBulletMax = 6;
	//bullets_.resize(kBulletMax);
	//specialBullets_.resize(kBulletMax);
	//bulletUIs_.resize(kBulletMax);
	//for (size_t i = 0; i < kBulletMax; ++i) {
	//	// 通常弾
	//	bullets_[i] = std::make_unique<PlayerBullet>();
	//	bullets_[i]->SetItem(player->GetItem());
	//	bullets_[i]->Init("PlayerBullet");
	//	bullets_[i]->SetPlayerShot(this);
	//	// 必殺技の弾
	//	specialBullets_[i] = std::make_unique<PlayerBullet>();
	//	specialBullets_[i]->SetItem(player->GetItem());
	//	specialBullets_[i]->Init("PlayerBulletSpecial");
	//	specialBullets_[i]->SetPlayerShot(this);
	//	// 弾UIを初期化
	//	bulletUIs_[i] = std::make_unique<PlayerBulletUI>();
	//	bulletUIs_[i]->Init(Vector2{});
	//}
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
	Collider::myType_ = ColliderType::kOBB;
	Collider::colliderName_ = "PlayerShotRay";
	Collider::isActive_ = true;
	Collider::targetColliderName_ = { "Enemy","DeadTree" };
	Collider::DrawCollider();

	rayReticle_ = std::make_unique<PlayerRayReticle>();
	rayReticle_->Init();
}

void PlayerShot::Update()
{
	// 今リロードが終わっている弾のを取得する
	kBulletCount_ = 0;
	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
		// 弾のリロードが終わっているならカウントに追加
		if (bullet->GetIsReload()) { ++kBulletCount_; }
	}
	for (auto& bullet : specialBullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
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
	killCountUI_->MochiPuniScale(kNockdownCount_);
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
	//// 動きが終わっている弾からリロードをする
	//for (auto& bullet : bullets_) {
	//	if (!bullet->GetIsActive() && !bullet->GetIsReload()) {
	//		WorldTransform transform;
	//		transform.rotation_ = rightStickQuaternion_;
	//		transform.translation_ = player_->GetTransform().translation_;
	//		bullet->Reload(transform, true);
	//		Input::GetInstance()->Vibrate(0.3f, 0.6f, 60);
	//		break;
	//	}
	//}

	isGatherRequested_ = true;
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
	/*for (auto& bullet : bullets_) {
		if (!bullet->GetIsActive() && bullet->GetIsReload()) {
			WorldTransform transform;
			transform.rotation_ = rightStickQuaternion_;
			transform.translation_ = player_->GetTransform().translation_;
			bullet->Attack(transform, player_->GetItem()->GetBulletData().speed);
			Input::GetInstance()->Vibrate(0.5f, 0.9f, 90);
			++kHitRate_;
			break;
		}
	}*/

	isShot_ = true;
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
	player_->SetTransformRotation(transform.rotation_);
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

	if (!rayHitCollider_) {
		rayHitCollider_ = other;
	} else {
		float distanceNow = Vector3::Distance(player_->GetTransform().translation_, rayHitCollider_->GetCenterPosition());
		float distanceOther = Vector3::Distance(player_->GetTransform().translation_, other->GetCenterPosition());
		if (distanceOther < distanceNow) {
			rayHitCollider_ = other;
		}
	}
}

void PlayerShot::RayUpdate()
{
	// 右スティックの入力を取得
	Vector3 rotateVelocity{};
	Input::GetInstance()->SetGamepadStickDeadzoneScale(0.5f);
	rotateVelocity.x = Input::GetInstance()->GetGamepadRightStickX();
	rotateVelocity.z = Input::GetInstance()->GetGamepadRightStickY();
	Input::GetInstance()->SetGamepadStickDeadzoneScale(1.0f);

	// 右スティックの入力があるならその方向にRayを向ける
	Quaternion rightQuaternion = Quaternion::IdentityQuaternion();
	if (rotateVelocity.Length() > 0.01f) {
		rayDirection_ = rotateVelocity;
		Collider::isActive_ = true;
	} else {
		Collider::isActive_ = false;
	}
	rightQuaternion = Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, rayDirection_, 0.3f);
	const auto& itemData = player_->GetItem()->GetPreObjectData();
	const auto rotateMatrix = Quaternion::MakeRotateMatrix(rightQuaternion);
	auto centerPosition = player_->GetTransform().translation_ + itemData.rayColliderPosition.Transform(rotateMatrix);

	// コライダーの更新
	Collider::size_ = itemData.rayColliderSize;
	Collider::rotate_ = rightQuaternion;
	Collider::centerPosition_ = centerPosition;
	Collider::Update();

	rayReticle_->SetRaticleAlpha(isRayHit_);
	if (rayHitCollider_) {
		rayReticle_->SetPosition(rayHitCollider_->GetCenterPosition());
	}
	rayReticle_->Update();

	ResetRayHit();
}
