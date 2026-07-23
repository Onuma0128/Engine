#include "GamePlayScene.h"

#include <algorithm>
#include <cmath>

#include "DeltaTimer.h"
#include "Input.h"
#include "SceneManager.h"
#include "BackEnd/httpClient.h"

void GamePlayScene::Initialize()
{
    dumbbellSensorController_ = std::make_unique<DumbbellSensorController>();
    dumbbellSensorController_->Initialize();

    numbersUI_ = std::make_unique<NumbersUI>();
    numbersUI_->Initialize();
    numbersUI_->SetPosition({ 115.0f, 64.0f });
    numbersUI_->SetDigitSize({ 128.0f, 128.0f });
    numbersUI_->SetDigitSpacing(-38.0f);

    dumbbellArm_ = std::make_unique<DumbbellArmUI>();
    dumbbellArm_->Initialize();

    cheerForUI_ = std::make_unique<CheerForUI>();
    cheerForUI_->Initialize();

    armSettingUI_ = std::make_unique<BaseUI>();
    armSettingUI_->Init("ArmSettingUI", "GameData");

    settingStartUI_ = std::make_unique<BaseUI>();
    settingStartUI_->Init("SettingStartUI", "GameData");

    gameStartState_ = GameStartState::WaitingForPoseCalibration;
    startCountdownTimer_ = 0.0f;
    gameTimer_ = 0.0f;

    startCountdownUI_ = std::make_unique<NumbersUI>();
    startCountdownUI_->Initialize(1);
    startCountdownUI_->SetPosition({ 640.0f, 260.0f });
    startCountdownUI_->SetDigitSize({ 128.0f, 128.0f });
    startCountdownUI_->SetVisible(false);
    startCountdownUI_->Update(3);

    gameTimerUI_ = std::make_unique<NumbersUI>();
    gameTimerUI_->Initialize(2);
    gameTimerUI_->SetPosition({ 640.0f, 40.0f });
    gameTimerUI_->SetDigitSize({ 80.0f, 80.0f });
    gameTimerUI_->SetDigitSpacing(-24.0f);
    gameTimerUI_->SetVisible(false);
    gameTimerUI_->Update(0);

    sceneFade_ = std::make_unique<BaseUI>();
    sceneFade_->Init("TitleFade", "GameData", true);
    sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
    sceneFade_->FadeOut();
}

void GamePlayScene::Finalize()
{
    dumbbellSensorController_->Finalize();
    dumbbellArm_->Finalize();
    numbersUI_->Finalize();
    startCountdownUI_->Finalize();
    gameTimerUI_->Finalize();
    cheerForUI_->Finalize();
}

void GamePlayScene::Update()
{
    dumbbellSensorController_->Update();
    UpdateGameStartFlow();

    numbersUI_->Update(dumbbellSensorController_->GetDumbbellCount());
    dumbbellArm_->Update(dumbbellSensorController_->GetExtendedAngle());
    cheerForUI_->Update(dumbbellSensorController_->GetDumbbellState());

    settingStartUI_->Update();
    armSettingUI_->Update();
    sceneFade_->Update();

    if (isFade_ && !sceneFade_->IsPlayAnimation())
    {
        SceneManager::GetInstance()->ChangeScene("Clear");
    }
}

void GamePlayScene::Draw()
{
    settingStartUI_->Draw();
    armSettingUI_->Draw();

    sceneFade_->Draw();
}

float GamePlayScene::GetSensorExtendedAngle() const
{
    return dumbbellSensorController_->GetExtendedAngle();
}

float GamePlayScene::GetSensorBentAngle() const
{
    return dumbbellSensorController_->GetBentAngle();
}

bool GamePlayScene::CanStartGame() const
{
    return dumbbellSensorController_ != nullptr &&
        dumbbellSensorController_->IsCalibrationComplete();
}

void GamePlayScene::StartCountdown()
{
    gameStartState_ = GameStartState::Countdown;
    startCountdownTimer_ = kStartCountdownSeconds;
    gameTimer_ = kGameTimerSeconds;

    startCountdownUI_->SetVisible(true);
    gameTimerUI_->SetVisible(false);
}

void GamePlayScene::UpdateGameStartFlow()
{
    Input* input = Input::GetInstance();
    const float deltaTime = DeltaTimer::GetDeltaTime();

    switch (gameStartState_)
    {
    case GameStartState::WaitingForPoseCalibration:
        startCountdownUI_->SetVisible(false);
        gameTimerUI_->SetVisible(false);

        if (CanStartGame() && input->TriggerKey(DIK_SPACE))
        {
            StartCountdown();
        }
        break;

    case GameStartState::Countdown:
    {
        startCountdownTimer_ = std::max(0.0f, startCountdownTimer_ - deltaTime);

        const int countdownNumber = std::max(
            1,
            static_cast<int>(std::ceil(startCountdownTimer_))
        );
        startCountdownUI_->Update(countdownNumber);

        if (startCountdownTimer_ <= 0.0f)
        {
            gameStartState_ = GameStartState::Playing;
            startCountdownUI_->SetVisible(false);
            gameTimerUI_->SetVisible(true);
            gameTimerUI_->Update(static_cast<int>(kGameTimerSeconds));
        }
        break;
    }

    case GameStartState::Playing:
    {
        gameTimer_ = std::max(0.0f, gameTimer_ - deltaTime);
        gameTimerUI_->Update(static_cast<int>(std::ceil(gameTimer_)));

        if (gameTimer_ <= 0.0f)
        {
            gameStartState_ = GameStartState::Finished;
            gameTimerUI_->Update(0);
            isFade_ = true;
            sceneFade_->FadeIn();
            auto count = dumbbellSensorController_->GetDumbbellCount();
            if (count > 0) {
                PostWorkout(count);
            }
        }
        break;
    }

    case GameStartState::Finished:
        break;
    }
}