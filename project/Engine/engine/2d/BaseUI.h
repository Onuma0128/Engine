#pragma once

#include <memory>
#include <string>

#include "Sprite.h"
#include "JsonFunction.h"

class BaseUI
{
private:

	struct UI_Parameters
	{
		std::string filename;
		std::string texture;

		Vector2 anchorPoint;
		Transform2D transform;

		bool isAnimation;
		float animationTime;
		Transform2D animaTransform;

		int easingType = 0;
	};

public:

	void Init(const std::string UI_Name);

	virtual void Update();

	virtual void Draw();

	void DrawImGui();

	void FadeIn();
	void FadeOut();

private:

	void UI_Animation();

	void Save();

protected:

	std::unique_ptr<Sprite> ui_ = nullptr;

	UI_Parameters parameters_;

	JsonFunction json_;

	// 選択中インデックス
	int textureIndex_ = 0;

	// Animation中のTimerとフラグ
	float playAnimationTimer_ = 0.0f;
	bool isPlayAnimation_ = false;
	// 逆再生
	bool reversePlayBack_ = false;

};

