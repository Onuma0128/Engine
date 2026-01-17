#include "SelectSystem.h"

#include "Input.h"
#include "DeltaTimer.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "GameCamera/GameCamera.h"
#include "Objects/Boss/Base/BossEnemy.h"
#include "Objects/Player/Player.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"
#include "Objects/MuscleCompanion/Manager/MuscleCompanionManager.h"
#include "Uis/GameSceneUIs/GameSceneUIs.h"

void SelectSystem::Init()
{
	// 背景Sprite
	selectUIs_[0] = std::make_unique<SelectUI>();
	selectUIs_[0]->Init("SelectBackWidth");
	selectUIs_[1] = std::make_unique<SelectUI>();
	selectUIs_[1]->Init("SelectBackHeight");
	// 背景の上のUISprite(キルのUI)
	selectUIs_[2] = std::make_unique<SelectUI>();
	selectUIs_[2]->Init("KillCountUI");
	// セレクト表示後の選択ボタンUI(タイトルともう一度)
	selectUIs_[3] = std::make_unique<SelectUI>();
	selectUIs_[3]->Init("SelectTitleUI");
	selectUIs_[3]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[4] = std::make_unique<SelectUI>();
	selectUIs_[4]->Init("SelectOnceAgainUI");
	selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[5] = std::make_unique<SelectUI>();
	selectUIs_[5]->Init("SelectRematchUI");
	selectUIs_[5]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	// 背景の上のUISprite(キルのUI)
	selectUIs_[6] = std::make_unique<SelectUI>();
	selectUIs_[6]->Init("CountUI");

	// キル数の表示UI
	killCountUI_ = std::make_unique<NumberCountUI>();
	killCountUI_->Init();
	killCountUI_->SetAlpha(0.0f);

	// 調整項目のロードと初期化
	items_ = std::make_unique<SelectAdjustItem>();
	items_->LoadItems();

	targetIndex_ = 1u;
}

void SelectSystem::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	// プレイヤーが死んだかクリアをしたらセレクトUIを表示する
	if ((!player_->GetIsAlive() || boss_->GetBossState() == BossState::Dead) && !isFadeIn_) {
		SelectUIFadeIn();
	}

	// ターゲットされているボタンを更新する
	const uint32_t kAddIndex = 3u;
	uint32_t index = targetIndex_ + kAddIndex;
	selectUIs_[index]->Blinking();

	for (auto& back : selectUIs_) {
		back->Update();
	}

	SelectInput();

	CounterUiUpdate();
}

void SelectSystem::Draw()
{
	if (isFadeIn_) {
		for (auto& back : selectUIs_) {
			back->Draw();
		}
		killCountUI_->Draw();
	}
}

void SelectSystem::SelectUIFadeIn()
{
	for (auto& back : selectUIs_) {
		back->FadeIn();
	}
	isFadeIn_ = true;
}

void SelectSystem::SelectInput()
{
	// シーンフェードインが完了したらシーン遷移
	if (isSceneFadeIn_ && !gameSceneUis_->GetIsFadePlayAnimation()) {
		if (targetIndex_ == 0u) {
			SceneManager::GetInstance()->ChangeScene("Title");
		} else {
			if (targetIndex_ == 1u) {
				boss_->Reset();
				spawner_->CountReset();
				gameSceneUis_->BossFadeReset();
				camera_->BossCameraReset();
				gameSceneUis_->SetDrawGameUIs(true);
			} else {
				boss_->StartBossEnemy();
				camera_->BossCameraEnd();
				gameSceneUis_->SetDrawGameUIs(false);
			}
			player_->Reset();
			spawner_->Reset();
			companionManager_->Reset();
			this->Reset();
			gameSceneUis_->GameSceneUIFadeOut();
		}
	}
	// セレクトUIが表示されていなければ処理しない
	if (!updateSelectUI_ || isSceneFadeIn_) { return; }

	Input* input = Input::GetInstance();
	// スティックを倒しているか
	const float leftStickX = input->GetGamepadLeftStickX();
	if (leftStickX < 0.0f || leftStickX > 0.0f) {
		targetTime_ += DeltaTimer::GetDeltaTime();
	} else {
		targetTime_ = 1.0f;
	}
	// ボタンを選択
	if (targetTime_ > 1.0f) {
		if (leftStickX < 0.0f && targetIndex_ != 0u) {
			--targetIndex_;
		} else if (leftStickX > 0.0f && targetIndex_ != 2u) {
			++targetIndex_;
		}
		const uint32_t killCount = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
		if (boss_->GetBossState() != BossState::Idle) {
			targetIndex_ = std::clamp(targetIndex_, 0u, 2u);
		} else {
			targetIndex_ = std::clamp(targetIndex_, 0u, 1u);
		}
		targetTime_ = 0.0f;
	}

	// ターゲットしていないUIはAlphaを下げる
	const Vector4 setColor = Vector4{ 1.0f,1.0f,1.0f,0.1f };
	if (targetIndex_ == 0u) {
		selectUIs_[4]->GetSprite()->SetColor(setColor);
		selectUIs_[5]->GetSprite()->SetColor(setColor);
	} else if (targetIndex_ == 1u) {
		selectUIs_[3]->GetSprite()->SetColor(setColor);
		selectUIs_[5]->GetSprite()->SetColor(setColor);
	} else {
		selectUIs_[3]->GetSprite()->SetColor(setColor);
		selectUIs_[4]->GetSprite()->SetColor(setColor);
	}

	// ボタンを押したらシーン遷移する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		isSceneFadeIn_ = true;
		gameSceneUis_->GameSceneUIFadeIn();
	}
}

void SelectSystem::CounterUiUpdate()
{
	// 数字をフェードさせる
	const auto& data = items_->GetSelectData();
	if (isFadeIn_ && !updateSelectUI_) {
		clearCountUiTimer_ += DeltaTimer::GetDeltaTime();
		killCountUI_->SetAlpha(clearCountUiTimer_ - 2.0f);
		if (clearCountUiTimer_ > data.selectUiInterval) {
			updateSelectUI_ = true;
			clearCountUiTimer_ = 0.0f;
		}
	}
	// 敵のキル数を取得する
	const uint32_t killCount = spawner_->GetKnockdownCount();

	// ランダムで数字を決める
	uint32_t first{};
	uint32_t second{};
	uint32_t third{};
	uint32_t result{ killCount };

	// 終了していなければ時間を足していく
	if (countUiOrder_ != CountUiOrder::End) {
		// ランダムで数字を決める
		std::mt19937 randomEngine(seedGenerator_());
		std::uniform_int_distribution<uint32_t> number(1, 9);
		first = (number(randomEngine));
		second = (number(randomEngine));
		third = (number(randomEngine));
		result = first + (second * 10) + (third * 100);
		if (updateSelectUI_) { clearCountUiTimer_ += DeltaTimer::GetDeltaTime(); }
	}

	switch (countUiOrder_)
	{
	case SelectSystem::CountUiOrder::First:
	{
		// 一桁目を固定
		first = killCount % 10;
	}
		break;
	case SelectSystem::CountUiOrder::Second:
	{
		// 二桁目まで固定
		first = killCount % 10;
		second = (killCount / 10) % 10;
	}
		break;
	case SelectSystem::CountUiOrder::Third:
	{
		// 三桁目まで固定
		first = killCount % 10;
		second = (killCount / 10) % 10;
		third = (killCount / 100) % 10;
	}
		break;
	default:
		break;
	}

	// Endまでenumを変化させる
	if (countUiOrder_ != CountUiOrder::End && updateSelectUI_) {
		if (items_->GetSelectData().clearCountUiTimer_ < clearCountUiTimer_) {
			clearCountUiTimer_ = 0.0f;
			int countType = static_cast<int>(countUiOrder_) + 1;
			countUiOrder_ = static_cast<CountUiOrder>(countType);
		}
		result = first + (second * 10) + (third * 100);
	}

	// 調整項目をセットし、更新する
	killCountUI_->SetInterval(items_->GetSelectData().killNumberUiInterval);
	killCountUI_->SetSize(items_->GetSelectData().killNumberUiSize);
	killCountUI_->SetPosition(items_->GetSelectData().killNumberUiPos);
	killCountUI_->Update(result);
}

void SelectSystem::Reset()
{
	isFadeIn_ = false;
	updateSelectUI_ = false;
	isSceneFadeIn_ = false;
	countUiOrder_ = CountUiOrder::First;
	selectUiInterval_ = 0.0f;
	clearCountUiTimer_ = 0.0f;
}