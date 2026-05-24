#include "GamePlayScene.h"

#include "BackEnd/httpClient.h"
#include "BackEnd/WriteUtf8.h"

void GamePlayScene::Initialize()
{
	// POST送信
	auto future = PostFacultyAsync("ゲームクリエイター科二年制");
	while (true) {
		if (future.valid() &&
			future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			std::string res = future.get();
			WriteUtf8("実行結果:" + res);

			// POST完了後に全権取得
			auto getFuture = GetAllFacultiesAsync();
			std::string all = getFuture.get();
			WriteUtf8("現在の全学科一覧:\n" + all);

			break;
		}
	}
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	
}

void GamePlayScene::Draw()
{
	
}