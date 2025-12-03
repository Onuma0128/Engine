#pragma once

#include <memory>

#include "AbstractSceneFactory.h"

/// <summary>
/// シーンファクトリークラス
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:

	// シーンを生成
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName)override;
};

