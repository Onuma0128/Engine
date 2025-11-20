#include "SceneFactory.h"
#include "titleScene/TitleScene.h"
#include "gameScene/GamePlayScene.h"
#include "gameClear/GameClearScene.h"
#include "gameOver/GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "Title") {
		newScene = new TitleScene();
	}else if (sceneName == "Game") {
		newScene = new GamePlayScene();
	}else if (sceneName == "Clear") {
		newScene = new GameClearScene();
	}else if (sceneName == "Over") {
		newScene = new GameOverScene();
	}

	return newScene;
}
