#pragma once

#include <string>
#include <memory>

#include "BaseScene.h"

namespace NumaEngine {

/// <summary>
/// シーンファクトリーの抽象クラス
/// </summary>
class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;

};

} // namespace NumaEngine


