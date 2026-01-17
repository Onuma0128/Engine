#include "MuscleCompanionManager.h"

#include "Objects/Player/Player.h"
#include "GameCamera/GameCamera.h"
#include "Objects/MuscleCompanion/State/CompanionDashState.h"

void MuscleCompanionManager::Initialize()
{
	// 調整項目の初期化
	items_ = std::make_unique<CompanionAdjustItem>();
	items_->LoadItems();
	audio_ = std::make_unique<Audio>();
	for (int i = 0; i < 8; ++i) {
		std::unique_ptr<MuscleCompanion> companion = std::make_unique<MuscleCompanion>();
		companions_.push_back(std::move(companion));
	}

	// 仲間の初期化
	uint32_t count = 0;
	for (auto& companion : companions_) {
		companion->SetPlayer(player_);
		companion->SetMapData(mapData_);
		companion->SetItems(items_.get());
		companion->SetCamera(camera_);
		companion->SetAudio(audio_.get());
		companion->Initialize();
		companion->SetTransformTranslation(Vector3::ExprUnitZ * static_cast<float>(count));
		count++;
	}

	// 予測オブジェクトの初期化
	predictionObjects_ = std::make_unique<PredictionObjects>();
	predictionObjects_->SetItems(items_.get());
	predictionObjects_->Init();

	// 矢印エフェクトの初期化
	arrowEffect_ = std::make_unique<NextArrowEffect>();
	arrowEffect_->SetItems(items_.get());
	arrowEffect_->Init();
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

	// エフェクトの更新
	UpdateEffect();
}

void MuscleCompanionManager::Draw()
{
	// エフェクトの描画
	arrowEffect_->Draw();
}

void MuscleCompanionManager::GatherCompanions()
{
	// プレイヤーが仲間を発射させたら
	if (player_->GetShot()->GetIsShot()) {
		player_->GetShot()->SetIsShot(IsShotCompanion());
	}
	player_->GetShot()->SetIsCanAttack(IsShotCompanion());
	// 仲間の中から発射できて近い仲間を発射する
	if (player_->GetShot()->GetIsShot()) {
		// データを取得する
		Vector3 playerPosition = player_->GetTransform().translation_;
		float preDistance = 1000.0f;
		MuscleCompanion* target = nullptr;
		// 一番近い仲間を発射する
		for (auto& companion : companions_) {
			// 発射できる状態じゃなければスキップする
			if (!companion->GetGatherRequested() || companion->GetState() == CharacterState::Dead) {
				continue;
			}
			// プレイヤーとの距離を測る
			float distance = Vector3::Distance(playerPosition, companion->GetTransform().translation_);
			// 距離が短ければ距離を更新する
			if (preDistance > distance) {
				preDistance = distance;
				target = companion.get();
			}
		}
		// ポインタが取れたらダッシュさせる
		if (target) { target->SetDashAttack(true); }
	}

	// プレイヤーから集合要求が来ていたら
	if (player_->GetShot()->GetGatherRequested()) {
		for (auto& companion : companions_) {
			if (companion->GetState() == CharacterState::Idle) {
				companion->SetGatherRequested(true);
			}
			if (!companion->GetReturnOriginal() && !audio_->IsPlaying("MattyoSet.wav") &&
				companion->GetState() != CharacterState::Dead) {
				audio_->SoundPlayWave("MattyoSet.wav", items_->GetSeVolumeData().set);
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

void MuscleCompanionManager::UpdateEffect()
{
	// エフェクト用の座標を取得する
	MuscleCompanion* target = nullptr;
	Vector3 position = {};
	Vector3 playerPosition = player_->GetTransform().translation_;
	float preDistance = 1000.0f;
	bool isDraw = false;

	// 一番近い仲間を探す
	for (auto& companion : companions_) {		
		if (!isDraw && companion->GetGatherRequested() && companion->GetState() != CharacterState::Dead) {
			// プレイヤーとの距離を測る
			float distance = Vector3::Distance(playerPosition, companion->GetTransform().translation_);
			// 距離が短ければ距離を更新する
			if (preDistance > distance) {
				preDistance = distance;
				target = companion.get();
			}
		}
		companion->SetOutLineColor(Vector3::ExprZero);
	}
	// ポインタがあるなら矢印を描画する
	if (target) {
		target->SetOutLineColor(Vector3::ExprUnitX);
		position = target->GetTransform().translation_;
		isDraw = true;

		// 向きをプレイヤーと同じにする
		const float distance = target->GetItems()->GetDashData().dashTargetDistance;
		Matrix4x4 rotate = Quaternion::MakeRotateMatrix(Quaternion::ExtractYawQuaternion(player_->GetTransform().rotation_));
		Vector3 targetPosition = player_->GetTransform().translation_;
		targetPosition += (Vector3::ExprUnitZ * distance).Transform(rotate);

		if (player_->GetShot()->GetTargetCollider()) {
			const auto* targetCollider_ = player_->GetShot()->GetTargetCollider();
			targetPosition = targetCollider_->GetCenterPosition();
		}
		predictionObjects_->Update(target->GetTransform().translation_, targetPosition);

	} else {
		isDraw = false;
	}

	arrowEffect_->SetDraw(isDraw);
	arrowEffect_->Update(position);
	predictionObjects_->SetDraw(isDraw);
}

void MuscleCompanionManager::Reset()
{
	uint32_t count = 0;
	for (auto& companion : companions_) {
		companion->Reset();
		companion->SetTransformTranslation(Vector3::ExprUnitZ * static_cast<float>(count));
		count++;
	}
}
