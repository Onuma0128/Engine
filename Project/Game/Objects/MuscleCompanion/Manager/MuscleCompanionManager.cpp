#include "MuscleCompanionManager.h"

#include "Objects/Player/Player.h"

void MuscleCompanionManager::Initialize()
{
	items_ = std::make_unique<CompanionAdjustItem>();
	items_->LoadItems();
	for (int i = 0; i < 5; ++i) {
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
		bool isShot = false;
		for (auto& companion : companions_) {
			if(companion->GetGatherRequested() != false){
				isShot = true;
				break;
			}
		}
		player_->GetShot()->SetIsShot(isShot);
	}

	// プレイヤーの弾から集合要求が来ていたら
	if (player_->GetShot()->GetGatherRequested()) {
		for (auto& companion : companions_) {
			companion->SetGatherRequested(true);
		}
		player_->GetShot()->SetGatherRequested(false);
	}
}
