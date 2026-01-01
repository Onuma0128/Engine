#include "SelectSystem.h"

#include "Input.h"
#include "DeltaTimer.h"
#include "SceneManager.h"

void SelectSystem::Init()
{
	// 背景Sprite
	selectUIs_[0] = std::make_unique<SelectUI>();
	selectUIs_[0]->Init("SelectBackWidth");
	selectUIs_[1] = std::make_unique<SelectUI>();
	selectUIs_[1]->Init("SelectBackHeight");
	// 背景の上のUISprite(キルと命中率のUI)
	selectUIs_[2] = std::make_unique<SelectUI>();
	selectUIs_[2]->Init("KillCountUI");
	// セレクト表示後の選択ボタンUI(タイトルともう一度)
	selectUIs_[3] = std::make_unique<SelectUI>();
	selectUIs_[3]->Init("SelectTitleUI");
	selectUIs_[3]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[4] = std::make_unique<SelectUI>();
	selectUIs_[4]->Init("SelectOnceAgainUI");
	selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	// 背景の上のUISprite(キルと命中率のUI)
	selectUIs_[5] = std::make_unique<SelectUI>();
	selectUIs_[5]->Init("CountUI");

	// キル数の表示UI
	killCountUI_ = std::make_unique<NumberCountUI>();
	killCountUI_->Init();
	killCountUI_->SetAlpha(0.0f);

	// 調整項目のロードと初期化
	items_ = std::make_unique<SelectAdjustItem>();
	items_->LoadItems();

	targetIndex_ = 0u;
}

void SelectSystem::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

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
	for (auto& back : selectUIs_) {
		back->Draw();
	}

	if (isFadeIn_) {
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
	if (isSceneFadeIn_ && !sceneFade_->IsPlayAnimation()) {
		if (targetIndex_ == 0u) {
			SceneManager::GetInstance()->ChangeScene("Title");
		} else {
			SceneManager::GetInstance()->ChangeScene("Game");
		}
	}
	// セレクトUIが表示されていなければ処理しない
	if (!updateSelectUI_ || isSceneFadeIn_) { return; }

	Input* input = Input::GetInstance();

	// ボタンを選択
	if (input->GetGamepadLeftStickX() < 0) {
		targetIndex_ = 0u;
	} else if (input->GetGamepadLeftStickX() > 0) {
		targetIndex_ = 1u;
	}

	// ターゲットしていないUIはAlphaを下げる
	if (targetIndex_ == 0u) {
		selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	} else {
		selectUIs_[3]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	}

	// ボタンを押したらシーン遷移する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		isSceneFadeIn_ = true;
		sceneFade_->FadeIn();
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

	// ランダムで数字を決める
	int first{};
	int second{};
	int third{};
	int result{ killCount_ };

	// 終了していなければ時間を足していく
	if (countUiOrder_ != CountUiOrder::End) {
		// ランダムで数字を決める
		std::mt19937 randomEngine(seedGenerator_());
		std::uniform_int_distribution<uint32_t> number(0, 9);
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
		first = killCount_ % 10;
	}
		break;
	case SelectSystem::CountUiOrder::Second:
	{
		// 二桁目まで固定
		first = killCount_ % 10;
		second = (killCount_ / 10) % 10;
	}
		break;
	case SelectSystem::CountUiOrder::Third:
	{
		// 三桁目まで固定
		first = killCount_ % 10;
		second = (killCount_ / 10) % 10;
		third = (killCount_ / 100) % 10;
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
