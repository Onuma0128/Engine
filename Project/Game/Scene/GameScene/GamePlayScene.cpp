#include "GamePlayScene.h"

#include "imgui.h"

#include "BackEnd/httpClient.h"
#include "BackEnd/WriteUtf8.h"

void GamePlayScene::Initialize()
{
	// POST送信
	postFuture_ = PostFacultyAsync(100);
	isRequesting_ = true;
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	ImGui::Begin("WabApi Faculties");
	if (isRequesting_) {
		ImGui::Text("WabApi へ登録中です...");
	} else if (isLoaded_) {
		ImGui::Text("POST結果");
		ImGui::Separator();
		ImGui::TextWrapped("%s", postResult_.c_str());

		ImGui::Spacing();
		ImGui::Text("学科一覧(JSON)");
		ImGui::Separator();
		ImGui::TextWrapped("%s", facultiesJson_.c_str());
	} else {
		ImGui::Text("まだデータはありません。");
	}
	ImGui::End();

	if (!isRequesting_ || !postFuture_.valid()) {
		return;
	}
	if (postFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
		return;
	}
	postResult_ = postFuture_.get();
	WriteUtf8("実行結果:" + postResult_ + "\n");

	auto getFuture = GetAllFacultiesAsync();
	facultiesJson_ = getFuture.get();
	WriteUtf8("現在の全学科一覧:\n" + facultiesJson_ + "\n");

	isRequesting_ = false;
	isLoaded_ = true;
}

void GamePlayScene::Draw()
{
	
}