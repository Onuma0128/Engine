#include "MuscleCompanionManager.h"

#include "Objects/Player/Player.h"

void MuscleCompanionManager::Initialize()
{
	items_ = std::make_unique<CompanionAdjustItem>();
	items_->LoadItems();
	for (int i = 0; i < 8; ++i) {
		std::unique_ptr<MuscleCompanion> companion = std::make_unique<MuscleCompanion>();
		companions_.push_back(std::move(companion));
	}

	uint32_t count = 0;
	for (auto& companion : companions_) {
		companion->SetPlayer(player_);
		companion->SetMapData(mapData_);
		companion->SetItems(items_.get());
		companion->Initialize();
		companion->SetTransformTranslation(Vector3::ExprUnitZ * static_cast<float>(count));
		count++;
	}
}

void MuscleCompanionManager::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	// 集合要求関数
	GatherCompanions();

	// 仲間同士の後続判定
	FollowerDistanceCollision();

	// キャラの更新を行う
	for (auto& companion : companions_) {
		companion->Update();
	}
}

void MuscleCompanionManager::Draw()
{

}

void MuscleCompanionManager::GatherCompanions()
{
	// プレイヤーが仲間を発射させたら
	if (player_->GetShot()->GetIsShot()) {
		player_->GetShot()->SetIsShot(IsShotCompanion());
	}
	player_->GetShot()->SetIsCanAttack(IsShotCompanion());

	// プレイヤーから集合要求が来ていたら
	if (player_->GetShot()->GetGatherRequested()) {
		for (auto& companion : companions_) {
			if (companion->GetState() == CharacterState::Idle) {
				companion->SetGatherRequested(true);
			}
		}
		player_->GetShot()->SetIsCanAttack(true);
		player_->GetShot()->SetGatherRequested(false);
	}
}

void MuscleCompanionManager::FollowerDistanceCollision()
{
	for (size_t i = 0; i < companions_.size(); ++i) {
		// 元の位置にいなければ飛ばす
		if (!companions_[i]->GetReturnOriginal() || companions_[i]->GetState() != CharacterState::Move) {
			companions_[i]->GetFollowerCollider()->SetActive(false);
			continue;
		}
		for (size_t j = i + 1; j < companions_.size(); ++j) {
			// 元の位置にいなければ飛ばす
			if (!companions_[j]->GetReturnOriginal() || companions_[j]->GetState() != CharacterState::Move){
				continue; 
			}
			// 距離を測る
			float distance = Vector3::Distance(
				companions_[i]->GetTransform().translation_,
				companions_[j]->GetTransform().translation_);
			const auto& data = companions_[i]->GetItems()->GetMainData();
			// 距離が近ければコライダーを有効にする
			if (distance < data.distanceToAlly) {
				companions_[i]->GetFollowerCollider()->SetActive(true);
				companions_[j]->GetFollowerCollider()->SetActive(true);
			} else {
				companions_[i]->GetFollowerCollider()->SetActive(false);
				companions_[j]->GetFollowerCollider()->SetActive(false);
			}
		}
	}
}

bool MuscleCompanionManager::IsShotCompanion()
{
	bool isShot = false;
	for (auto& companion : companions_) {
		if (companion->GetGatherRequested() && companion->GetState() != CharacterState::Dead) {
			isShot = true;
			break;
		}
	}
	return isShot;
}
