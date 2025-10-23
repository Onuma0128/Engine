#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// シーンファクトリーの抽象クラス
/// </summary>
class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

};

