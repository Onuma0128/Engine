#pragma once

#include "Sprite.h"

/// <summary>
/// シーンフェード
/// </summary>
class SceneFade : public Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードイン
	/// </summary>
	/// <param name="fadeTime"></フェードするフレームを決める>
	void FadeIn(const float fadeTime = 1.0f);

	/// <summary>
	/// フェードアウト
	/// </summary>
	/// <param name="fadeTime"></フェードするフレームを決める>
	void FadeOut(const float fadeTime = 0.0f);
	
	/// <summary>
	/// フェードしているか
	/// </summary>
	/// <returns></returns>
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

