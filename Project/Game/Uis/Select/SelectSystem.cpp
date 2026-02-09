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
	// セレクト表示後の選択ボタンUI(タイトルともう一度)
	selectUIs_[2] = std::make_unique<SelectUI>();
	selectUIs_[2]->Init("SelectTitleUI");
	selectUIs_[2]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[3] = std::make_unique<SelectUI>();
	selectUIs_[3]->Init("SelectOnceAgainUI");
	selectUIs_[3]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[4] = std::make_unique<SelectUI>();
	selectUIs_[4]->Init("SelectRematchUI");
	selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	// 背景の上のUISprite(キルのUI)
	selectUIs_[5] = std::make_unique<SelectUI>();
	selectUIs_[5]->Init("GameOverUI");
	selectUIs_[6] = std::make_unique<SelectUI>();
	selectUIs_[6]->Init("GameClearUI");

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
	if (!isFadeIn_) {
		if (!player_->GetIsAlive() || !companionManager_->IsAliveCompanion()) {
			this->SelectUIFadeIn(false);
		} else if (camera_->GetClearEnd()) {
			this->SelectUIFadeIn(true);
		}
	}

	// ターゲットされているボタンを更新する
	const uint32_t kAddIndex = 2u;
	uint32_t index = targetIndex_ + kAddIndex;
	selectUIs_[index]->Blinking();

	for (auto& back : selectUIs_) {
		back->Update();
	}

	SelectInput();

	// セレクトUIが表示されているか判定
	const auto& data = items_->GetSelectData();
	if (isFadeIn_ && !updateSelectUI_) {
		clearCountUiTimer_ += DeltaTimer::GetDeltaTime();
		if (clearCountUiTimer_ > data.selectUiInterval) {
			updateSelectUI_ = true;
			clearCountUiTimer_ = 0.0f;
		}
	}
}

void SelectSystem::Draw()
{
	if (isFadeIn_) {
		for (auto& back : selectUIs_) {
			back->Draw();
		}
	}
}

void SelectSystem::SelectUIFadeIn(bool isClear)
{
	for (size_t i = 0; i < kSelectUiSize_; ++i) {
		if (isClear && (i == 1 || i == 5)) { continue; }
		if (!isClear && i == 6) { continue; }
		selectUIs_[i]->FadeIn();
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
			player_->Reset();
			spawner_->Reset();
			companionManager_->Reset();
			this->Reset();
			gameSceneUis_->GameSceneUIFadeOut();
			if (targetIndex_ == 1u) {
				boss_->Reset();
				spawner_->CountReset();
				gameSceneUis_->BossFadeReset();
				camera_->BossCameraReset();
				gameSceneUis_->SetDrawGameUIs(true);
				const float kBGMVolume = 0.08f;
				audio_->SoundPlayWave("GameSceneBGM.wav", kBGMVolume, true);
			} else {
				boss_->StartBossEnemy();
				camera_->BossCameraEnd();
				gameSceneUis_->SetDrawGameUIs(false);
				const float kBGMVolume = 0.08f;
				audio_->SoundPlayWave("BossBGM.wav", kBGMVolume, true);
			}
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
		selectUIs_[3]->GetSprite()->SetColor(setColor);
		selectUIs_[4]->GetSprite()->SetColor(setColor);
	} else if (targetIndex_ == 1u) {
		selectUIs_[2]->GetSprite()->SetColor(setColor);
		selectUIs_[4]->GetSprite()->SetColor(setColor);
	} else {
		selectUIs_[2]->GetSprite()->SetColor(setColor);
		selectUIs_[3]->GetSprite()->SetColor(setColor);
	}

	// ボタンを押したらシーン遷移する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		isSceneFadeIn_ = true;
		gameSceneUis_->GameSceneUIFadeIn();
	}
}

void SelectSystem::Reset()
{
	isFadeIn_ = false;
	updateSelectUI_ = false;
	isSceneFadeIn_ = false;
	for (auto& back : selectUIs_) {
		back->FadeOut();
	}

	audio_->StopAudio("GameSceneBGM.wav");
	audio_->StopAudio("GameClearBGM.wav");
	audio_->StopAudio("BossBGM.wav");
}

void SelectSystem::BossStart()
{
	boss_->StartBossEnemy();
	camera_->BossCameraEnd();
	gameSceneUis_->SetDrawGameUIs(false);
	player_->Reset();
	spawner_->Reset();
	spawner_->SetSpawnCount(30);
	companionManager_->Reset();
	this->Reset();
	gameSceneUis_->GameSceneUIFadeOut();

	const float kBGMVolume = 0.08f;
	audio_->SoundPlayWave("BossBGM.wav", kBGMVolume, true);
}
