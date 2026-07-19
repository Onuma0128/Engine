#include "SceneFactory.h"

#include "GameScene/GamePlayScene.h"
#include "TitleScene/TitleScene.h"
#include "ClearScene/ClearScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	std::unique_ptr<BaseScene> newScene = nullptr;

	if (sceneName == "Title") {
		newScene = std::make_unique<TitleScene>();
	}else if (sceneName == "Game") {
		newScene = std::make_unique<GamePlayScene>();
	}else if (sceneName == "Clear") {
		newScene = std::make_unique<ClearScene>();
	}else if (sceneName == "Over") {
		newScene = std::make_unique<GamePlayScene>();
	}

	return newScene;
}
