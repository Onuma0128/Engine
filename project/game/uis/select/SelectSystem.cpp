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
	selectUIs_[3] = std::make_unique<SelectUI>();
	selectUIs_[3]->Init("HitRateUI");
	// セレクト表示後の選択ボタンUI(タイトルともう一度)
	selectUIs_[4] = std::make_unique<SelectUI>();
	selectUIs_[4]->Init("SelectTitleUI");
	selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	selectUIs_[5] = std::make_unique<SelectUI>();
	selectUIs_[5]->Init("SelectOnceAgainUI");
	selectUIs_[5]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	// 背景の上のUISprite(キルと命中率のUI)
	selectUIs_[6] = std::make_unique<SelectUI>();
	selectUIs_[6]->Init("CountUI");
	selectUIs_[7] = std::make_unique<SelectUI>();
	selectUIs_[7]->Init("RatioUI");

	// キル数の表示UI
	killCountUI_ = std::make_unique<PlayerCountUI>();
	killCountUI_->Init();
	killCountUI_->SetAlpha(0.0f);
	
	// 命中率の表示UI
	hitRateUI_ = std::make_unique<PlayerCountUI>();
	hitRateUI_->Init();
	hitRateUI_->SetAlpha(0.0f);

	// 調整項目のロードと初期化
	items_ = std::make_unique<SelectAdjustItem>();
	items_->LoadItems();

	targetIndex_ = 0u;
}

void SelectSystem::Update()
{
#ifdef _DEBUG
	items_->Editor();
#endif // _DEBUG

	// ターゲットされているボタンを更新する
	uint32_t index = targetIndex_ + 4u;
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
		hitRateUI_->Draw();
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
	if (!updateSelectUI_) { return; }

	Input* input = Input::GetInstance();

	// ボタンを選択
	if (input->GetGamepadLeftStickX() < 0) {
		targetIndex_ = 0u;
	} else if (input->GetGamepadLeftStickX() > 0) {
		targetIndex_ = 1u;
	}

	// ターゲットしていないUIはAlphaを下げる
	if (targetIndex_ == 0u) {
		selectUIs_[5]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	} else {
		selectUIs_[4]->GetSprite()->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.1f });
	}

	// ボタンを押したらシーン遷移する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		if (targetIndex_ == 0u) {
			SceneManager::GetInstance()->ChangeScene("Title");
		} else {
			SceneManager::GetInstance()->ChangeScene("Game");
		}
	}
}

void SelectSystem::CounterUiUpdate()
{
	// 数字をフェードさせる
	if (isFadeIn_ && !updateSelectUI_) {
		clearCountUiTimer_ += DeltaTimer::GetDeltaTime();
		killCountUI_->SetAlpha(clearCountUiTimer_ - 1.0f);
		hitRateUI_->SetAlpha(clearCountUiTimer_ - 1.0f);
		if (clearCountUiTimer_ > items_->GetSelectData().selectUiInterval) {
			updateSelectUI_ = true;
			clearCountUiTimer_ = 0.0f;
		}
	}

	// ランダムで数字を決める
	std::array<int, 2> first{};
	std::array<int, 2> second{};
	std::array<int, 2> third{};
	std::array<int, 2> result{ killCount_,hitRate_ };

	// 終了していなければ時間を足していく
	if (countUiOrder_ != CountUiOrder::End) {
		// ランダムで数字を決める
		std::mt19937 randomEngine(seedGenerator_());
		std::uniform_int_distribution<uint32_t> number(0, 9);
		for (size_t i = 0; i < first.size(); ++i) {
			first[i] = (number(randomEngine));
			second[i] = (number(randomEngine));
			third[i] = (number(randomEngine));
			result[i] = first[i] + (second[i] * 10) + (third[i] * 100);
		}
		if (updateSelectUI_) { clearCountUiTimer_ += DeltaTimer::GetDeltaTime(); }
	}

	switch (countUiOrder_)
	{
	case SelectSystem::CountUiOrder::First:
	{
		// 一桁目を固定
		first[0] = killCount_ % 10;
		first[1] = hitRate_ % 10;
	}
		break;
	case SelectSystem::CountUiOrder::Second:
	{
		// 二桁目まで固定
		first[0] = killCount_ % 10;
		first[1] = hitRate_ % 10;
		second[0] = (killCount_ / 10) % 10;
		second[1] = (hitRate_ / 10) % 10; 
	}
		break;
	case SelectSystem::CountUiOrder::Third:
	{
		// 三桁目まで固定
		first[0] = killCount_ % 10;
		first[1] = hitRate_ % 10;
		second[0] = (killCount_ / 10) % 10;
		second[1] = (hitRate_ / 10) % 10;
		third[0] = (killCount_ / 100) % 10;
		third[1] = (hitRate_ / 100) % 10;
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
		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = first[i] + (second[i] * 10) + (third[i] * 100);
		}
	}

	// 調整項目をセットし、更新する
	killCountUI_->SetInterval(items_->GetSelectData().killNumberUiInterval);
	killCountUI_->SetSize(items_->GetSelectData().killNumberUiSize);
	killCountUI_->SetPosition(items_->GetSelectData().killNumberUiPos);
	killCountUI_->Update(result[0]);

	hitRateUI_->SetInterval(items_->GetSelectData().hitRateNumberUiInterval);
	hitRateUI_->SetSize(items_->GetSelectData().hitRateNumberUiSize);
	hitRateUI_->SetPosition(items_->GetSelectData().hitRateNumberUiPos);
	hitRateUI_->Update(result[1]);
}
