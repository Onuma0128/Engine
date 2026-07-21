#pragma once

#include <array>
#include <future>
#include <memory>
#include <string>

#define NOMINMAX
#include "BaseScene.h"
#include "BaseUI.h"
#include "Uis/NumbersUI.h"

class ClearScene : public BaseScene
{
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:

    /// <summary>
    /// サーバーから受け取ったランキングを反映する
    /// </summary>
    void LoadRanking(const std::string& response);

private:

    static constexpr size_t kRankingCount = 5;

    // 順位の数字
    std::array<
        std::unique_ptr<NumbersUI>,
        kRankingCount
    > rankNumberUIs_;

    // 各順位のダンベル回数
    std::array<
        std::unique_ptr<NumbersUI>,
        kRankingCount
    > rankingCountUIs_;

    // ドアが開くアニメーション
    std::unique_ptr<BaseUI> doorOpenL_ = nullptr;
    std::unique_ptr<BaseUI> doorOpenR_ = nullptr;
    bool isDoorOpen_ = false;

    std::unique_ptr<BaseUI> backGround_ = nullptr;

    // サーバー通信結果
    std::future<std::string> rankingFuture_;

    // ランキング取得中か
    bool isRankingLoading_ = false;

    // ランキング取得完了
    bool isRankingLoaded_ = false;

    // フェード
    std::unique_ptr<BaseUI> sceneFade_ = nullptr;
    bool isFade_ = false;
};