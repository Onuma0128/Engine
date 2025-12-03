#include "SceneFactory.h"
#include "titleScene/TitleScene.h"
#include "gameScene/GamePlayScene.h"
#include "gameClear/GameClearScene.h"
#include "gameOver/GameOverScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	std::unique_ptr<BaseScene> newScene = nullptr;

	if (sceneName == "Title") {
		newScene = std::make_unique<TitleScene>();
	}else if (sceneName == "Game") {
		newScene = std::make_unique<GamePlayScene>();
	}else if (sceneName == "Clear") {
		newScene = std::make_unique<GameClearScene>();
	}else if (sceneName == "Over") {
		newScene = std::make_unique<GameOverScene>();
	}

	return newScene;
}
