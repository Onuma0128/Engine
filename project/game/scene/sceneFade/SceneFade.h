#pragma once

#include "Sprite.h"

class SceneFade : public Sprite
{
public:

	void Init();

	void Update();

	void Draw();

	// フェードをさせる
	void FadeIn(const float fadeTime = 1.0f);
	void FadeOut(const float fadeTime = 0.0f);
	// フェードしているか
	bool GetIsFade()const { return isFade_; }

private:

	// 今フェードをしているか
	bool isFade_ = false;
	// inかoutを行っているか
	bool isFade_in_ = true;
	bool isFade_out_ = false;
	// フェード中の時間
	float fadeTime_ = 0.0f;

};

