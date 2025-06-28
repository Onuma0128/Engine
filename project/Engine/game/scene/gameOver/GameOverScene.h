#pragma once

#include <memory>

#include "BaseScene.h"

#include "Camera.h"
#include "Animation.h"
#include "scene/sceneFade/SceneFade.h"

class GameOverScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<SceneFade> sceneFade_ = nullptr;
	bool isFade_ = false;

	std::unique_ptr<Animation> enemy_ = nullptr;

};

