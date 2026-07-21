#include "ClearScene.h"

#include <chrono>
#include <iostream>

#include "Input.h"
#include "SceneManager.h"
#include "BackEnd/httpClient.h"

#include "json.hpp"

using json = nlohmann::json;

void ClearScene::Initialize()
{
    backGround_ = std::make_unique<BaseUI>();
    backGround_->Init("TitleBackGround", "GameData");
    backGround_->GetSprite()->SetSceneRenderer();

    // -------------------------
    // 順位と回数のUIを作成
    // -------------------------
    for (size_t i = 0; i < kRankingCount; ++i)
    {
        // 1位～5位の数字
        rankNumberUIs_[i] =
            std::make_unique<NumbersUI>();

        rankNumberUIs_[i]->Initialize(1);

        rankNumberUIs_[i]->SetPosition({
            450.0f,
            180.0f + static_cast<float>(i) * 90.0f
            });

        rankNumberUIs_[i]->SetDigitSize({
            64.0f,
            64.0f
            });

        rankNumberUIs_[i]->Update(
            static_cast<int>(i) + 1
        );

        // 各順位の回数
        rankingCountUIs_[i] =
            std::make_unique<NumbersUI>();

        // 最大3桁として作成
        rankingCountUIs_[i]->Initialize(3);

        rankingCountUIs_[i]->SetPosition({
            700.0f,
            180.0f + static_cast<float>(i) * 90.0f
            });

        rankingCountUIs_[i]->SetDigitSize({
            64.0f,
            64.0f
            });

        rankingCountUIs_[i]->SetDigitSpacing(-16.0f);

        // 取得完了までは0を表示
        rankingCountUIs_[i]->Update(0);
    }

    // -------------------------
    // ランキング取得開始
    // -------------------------
    rankingFuture_ = GetWorkoutRankingAsync();
    isRankingLoading_ = true;
    isRankingLoaded_ = false;

    // -------------------------
    // フェード
    // -------------------------
    sceneFade_ = std::make_unique<BaseUI>();
    sceneFade_->Init(
        "TitleFade",
        "GameData",
        true
    );

    sceneFade_->GetSprite()->SetColor(
        Vector4{
            0.0f,
            0.0f,
            0.0f,
            1.0f
        }
    );

    doorOpenL_ = std::make_unique<BaseUI>();
    doorOpenL_->Init("doorOpenL", "GameData");
	doorOpenL_->GetSprite()->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, 0.0f });

    doorOpenR_ = std::make_unique<BaseUI>();
    doorOpenR_->Init("doorOpenR", "GameData");
    doorOpenR_->GetSprite()->SetFlipX(true);
    doorOpenR_->GetSprite()->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, 0.0f });

    sceneFade_->FadeOut();
}

void ClearScene::Finalize()
{
    for (auto& rankUI : rankNumberUIs_)
    {
        if (rankUI != nullptr)
        {
            rankUI->Finalize();
            rankUI.reset();
        }
    }

    for (auto& countUI : rankingCountUIs_)
    {
        if (countUI != nullptr)
        {
            countUI->Finalize();
            countUI.reset();
        }
    }

    sceneFade_.reset();
}

void ClearScene::Update()
{
    // -------------------------
    // ランキング通信の完了確認
    // -------------------------
    if (isRankingLoading_ &&
        rankingFuture_.valid())
    {
        const auto status =
            rankingFuture_.wait_for(
                std::chrono::seconds(0)
            );

        if (status == std::future_status::ready)
        {
            const std::string response =
                rankingFuture_.get();

            LoadRanking(response);

            isRankingLoading_ = false;
            isRankingLoaded_ = true;
        }
    }

    Input* input = Input::GetInstance();

    if (input->TriggerKey(DIK_SPACE) &&
        !isFade_)
    {
        isFade_ = true;
        sceneFade_->FadeIn();
        doorOpenL_->GetSprite()->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });
        doorOpenR_->GetSprite()->SetColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });
        doorOpenL_->FadeOut();
		doorOpenR_->FadeOut();
    }

    // フェードが終わったらシーン遷移する
    if (isFade_ &&
        !sceneFade_->IsPlayAnimation())
    {
        SceneManager::GetInstance()->
            ChangeScene("Title");
    }

    // -------------------------
    // UI更新
    // -------------------------
    doorOpenL_->Update();
    doorOpenR_->Update();
    backGround_->Update();
    sceneFade_->Update();
}

void ClearScene::Draw()
{
    doorOpenL_->Draw();
    doorOpenR_->Draw();
    sceneFade_->Draw();
}

void ClearScene::LoadRanking(
    const std::string& response
)
{
    try
    {
        const json rankingJson =
            json::parse(response);

        if (!rankingJson.is_array())
        {
            std::cerr
                << "ランキングJSONが配列ではありません\n";

            return;
        }

        // データが5件未満の場合に備えて
        // 最初に全部0へ戻す
        for (auto& countUI : rankingCountUIs_)
        {
            if (countUI != nullptr)
            {
                countUI->Update(0);
            }
        }

        const size_t rankingSize =
            std::min(
                rankingJson.size(),
                kRankingCount
            );

        for (size_t i = 0;
            i < rankingSize;
            ++i)
        {
            if (!rankingJson[i].contains("count"))
            {
                continue;
            }

            const int count =
                rankingJson[i]["count"].get<int>();

            rankingCountUIs_[i]->Update(count);

            std::cout
                << static_cast<int>(i + 1)
                << "位: "
                << count
                << "回\n";
        }
    }
    catch (const json::exception& error)
    {
        std::cerr
            << "ランキングJSON解析エラー: "
            << error.what()
            << '\n';

        std::cerr
            << "受信内容: "
            << response
            << '\n';
    }
}