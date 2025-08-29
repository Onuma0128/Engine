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
	};

public:

	void Init(const std::string UI_Name);

	virtual void Update();

	virtual void Draw();

	void DrawImGui();

private:

	void Save();

protected:

	std::unique_ptr<Sprite> ui_ = nullptr;

	UI_Parameters parameters_;

	JsonFunction json_;

	// 選択中インデックス
	int textureIndex_ = 0;
};

