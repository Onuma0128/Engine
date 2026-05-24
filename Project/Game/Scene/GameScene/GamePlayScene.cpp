#include "GamePlayScene.h"

#include "imgui.h"
#include <algorithm>
#include <chrono>

#include "BackEnd/httpClient.h"
#include "BackEnd/WriteUtf8.h"
#include "json.hpp"
using json = nlohmann::json;

void GamePlayScene::Initialize()
{
	state_ = State::WaitStart;
	elapsedSeconds_ = 0.0f;
	currentScore_ = 0;
	isTimerRunning_ = false;
	isPosting_ = false;
	isFetchingRanking_ = false;
	errorText_.clear();
	postResult_.clear();
	rankingText_.clear();
	rankingRawJson_.clear();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	if (state_ == State::Running && isTimerRunning_) {
		auto now = std::chrono::steady_clock::now();
		elapsedSeconds_ = std::chrono::duration<float>(now - startTime_).count();
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
		if (state_ == State::WaitStart) {
			StartTimer();
		} else if (state_ == State::Running) {
			StopTimer();
		} else if (state_ == State::Ranking && !isFetchingRanking_ && !isPosting_) {
			state_ = State::WaitStart;
			elapsedSeconds_ = 0.0f;
			currentScore_ = 0;
			errorText_.clear();
		}
	}



	if (ImGui::IsKeyPressed(ImGuiKey_Enter) && state_ == State::Result && !isPosting_ && !isFetchingRanking_) {
		BeginPostScore();
	}

	UpdateNetwork();

	ImGui::Begin("StopWatchGame");
	ImGui::Text("State: %s",
		state_ == State::WaitStart ? "WAIT_START" :
		state_ == State::Running ? "RUNNING" :
		state_ == State::Result ? "RESULT" : "RANKING");
	ImGui::Separator();

	if (state_ == State::WaitStart) {
		ImGui::Text("SPACEで開始");
	} else if (state_ == State::Running) {
		if (elapsedSeconds_ <= 7.0f) {
			ImGui::Text("sec: %.6f", elapsedSeconds_);
		} else {
			ImGui::Text("sec: ???");
		}
		ImGui::Text("SPACEで停止");
	} else if (state_ == State::Result) {
		ImGui::Text("停止時間: %.6f", elapsedSeconds_);
		ImGui::Text("Score: %d", currentScore_);
		ImGui::Text("ENTERでスコア送信");
	} else {
		if (isPosting_ || isFetchingRanking_) {
			ImGui::Text("通信中...");
		}
		if (!errorText_.empty()) {
			ImGui::TextWrapped("Error: %s", errorText_.c_str());
		}
		if (!rankingText_.empty()) {
			ImGui::Text("Top 5 Ranking");
			ImGui::Separator();
			ImGui::TextWrapped("%s", rankingText_.c_str());
		}
		ImGui::Text("SPACEでリスタート");
	}
	ImGui::End();
}

void GamePlayScene::Draw()
{
	
}

void GamePlayScene::StartTimer()
{
	startTime_ = std::chrono::steady_clock::now();
	elapsedSeconds_ = 0.0f;
	isTimerRunning_ = true;
	state_ = State::Running;
}

void GamePlayScene::StopTimer()
{
	auto now = std::chrono::steady_clock::now();
	elapsedSeconds_ = std::chrono::duration<float>(now - startTime_).count();
	isTimerRunning_ = false;
	currentScore_ = CalculateScore(elapsedSeconds_);
	state_ = State::Result;
}

int32_t GamePlayScene::CalculateScore(float elapsedSeconds) const
{
	if (elapsedSeconds > 10.0f) {
		return 0;
	}
	const float diff = std::abs(elapsedSeconds - 10.0f);
	const int32_t score = static_cast<int32_t>(1000.0f - diff * 1000.0f);
	return std::max(0, score);
}

void GamePlayScene::BeginPostScore()
{
	errorText_.clear();
	rankingText_.clear();
	rankingRawJson_.clear();
	postFuture_ = PostFacultyAsync(currentScore_);
	isPosting_ = true;
}

void GamePlayScene::BeginFetchRanking()
{
	rankingFuture_ = GetAllFacultiesAsync();
	isFetchingRanking_ = true;
}

void GamePlayScene::UpdateNetwork()
{
	if (isPosting_ && postFuture_.valid() &&
		postFuture_.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
		postResult_ = postFuture_.get();
		WriteUtf8("POST結果:" + postResult_ + "\n");
		isPosting_ = false;
		BeginFetchRanking();
	}

	if (isFetchingRanking_ && rankingFuture_.valid() &&
		rankingFuture_.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
		rankingRawJson_ = rankingFuture_.get();
		WriteUtf8("ランキング取得:\n" + rankingRawJson_ + "\n");
		isFetchingRanking_ = false;
		BuildRankingTextFromJson(rankingRawJson_);
		state_ = State::Ranking;
	}
}

void GamePlayScene::BuildRankingTextFromJson(const std::string& jsonText)
{
	try {
		auto data = json::parse(jsonText);
		if (!data.is_array()) {
			errorText_ = "JSONが配列ではありません";
			return;
		}

		std::vector<int32_t> scores;
		scores.reserve(data.size());
		for (const auto& item : data) {
			if (item.contains("score") && item["score"].is_number_integer()) {
				scores.push_back(item["score"].get<int32_t>());
			} else if (item.contains("scoer") && item["scoer"].is_number_integer()) {
				scores.push_back(item["scoer"].get<int32_t>());
			}
		}

		std::sort(scores.begin(), scores.end(), std::greater<int32_t>());
		if (scores.size() > 5) {
			scores.resize(5);
		}

		rankingText_.clear();
		for (size_t i = 0; i < scores.size(); ++i) {
			rankingText_ += std::to_string(i + 1) + "位 : " + std::to_string(scores[i]) + "\n";
		}
		if (rankingText_.empty()) {
			rankingText_ = "ランキングデータなし";
		}
	}
	catch (const std::exception& e) {
		errorText_ = std::string("ランキング解析失敗: ") + e.what();
	}

}