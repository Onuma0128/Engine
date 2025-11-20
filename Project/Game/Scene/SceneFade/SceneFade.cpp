#include "SceneFade.h"

#include "DeltaTimer.h"

void SceneFade::Init()
{
	Sprite::Initialize("white1x1.png");
	Sprite::SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	
	transform_.size = { 1280.0f,720.0f };
}

void SceneFade::Update()
{
	// フェードをしていなければ即リターン
	if (!isFade_) {
		Sprite::Update();
		return;
	}
	// フェードの処理
	float time = 0.0f;
	if (isFade_in_) {
		fadeTime_ -= DeltaTimer::GetDeltaTime();
		time = std::clamp(fadeTime_, 0.0f, 1.0f);
		if (time == 0.0f) { isFade_ = false; }
	}
	if (isFade_out_) {
		fadeTime_ += DeltaTimer::GetDeltaTime();
		time = std::clamp(fadeTime_, 0.0f, 1.0f);
		if (time == 1.0f) { isFade_ = false; }
	}

	Sprite::SetColor(Vector4{ 0.0f,0.0f,0.0f,time });
	Sprite::Update();
}

void SceneFade::Draw()
{
	Sprite::Draw();
}

void SceneFade::FadeIn(const float fadeTime)
{
	isFade_ = true;
	isFade_in_ = true;
	isFade_out_ = false;
	fadeTime_ = fadeTime;
}

void SceneFade::FadeOut(const float fadeTime)
{
	isFade_ = true;
	isFade_in_ = false;
	isFade_out_ = true;
	fadeTime_ = fadeTime;
}
