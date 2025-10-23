#pragma once
#include "AbstractSceneFactory.h"

/// <summary>
/// シーンファクトリークラス
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:

	// シーンを生成
	BaseScene* CreateScene(const std::string& sceneName)override;
};

