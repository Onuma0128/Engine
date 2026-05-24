#pragma once
#define NOMINMAX
#include <memory>
#include <future>
#include <string>
#include <vector>
#include <chrono>
#include <cstdint>

#include "BaseScene.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
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
	enum class State {
		WaitStart,
		Running,
		Result,
		Ranking,
	};

	void StartTimer();
	void StopTimer();
	int32_t CalculateScore(float elapsedSeconds) const;
	void BeginPostScore();
	void BeginFetchRanking();
	void UpdateNetwork();
	void BuildRankingTextFromJson(const std::string& jsonText);

	State state_ = State::WaitStart;
	std::chrono::steady_clock::time_point startTime_{};
	float elapsedSeconds_ = 0.0f;
	int32_t currentScore_ = 0;
	bool isTimerRunning_ = false;

	std::future<std::string> postFuture_;
	std::future<std::string> rankingFuture_;
	bool isPosting_ = false;
	bool isFetchingRanking_ = false;
	std::string postResult_;
	std::string rankingRawJson_;
	std::string rankingText_;
	std::string errorText_;

};

