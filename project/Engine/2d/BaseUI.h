#pragma once

#include <memory>
#include <string>

#include "Sprite.h"
#include "JsonFunction.h"

class BaseUI
{
private:

	struct UiParameters
	{
		std::string filename;			// 保存ファイル名
		std::string biginName;			// ImGuiの入れる名前
		std::string texture;			// テクスチャ名

		Vector2 anchorPoint;			// アンカーポイント
		Transform2D transform;			// トランスフォーム
		Vector4 color;					// カラー

		bool isAnimation;				// アニメーションを適応するか
		float animationTime;			// アニメーション時間
		Transform2D animaTransform;		// アニメーション後のトランスフォーム
		Vector4 animaColor;				// アニメーション後のカラー

		int inEasingType = 0;			// FadeInのEasingType
		int outEasingType = 0;			// FadeOutのEasingType
		float fadeInInterval;			// FadeInになる時の間隔
		float fadeOutInterval;			// FadeOutになる時の間隔

	};

public:

	void Init(const std::string uiName, const std::string biginName);

	virtual void Update();

	virtual void Draw();

	void DrawImGui();

	// FadeIn,FadeOutを呼び出し
	void FadeIn();
	void FadeOut();

	// 点滅させる
	void Blinking();
	// 点滅リセット
	void Reset();

	Sprite* GetSprite()const { return ui_.get(); }

private:

	void UI_Animation();

	void Save();

private:

	std::unique_ptr<Sprite> ui_ = nullptr;

	UiParameters parameters_;

	JsonFunction json_;

	// 選択中インデックス
	int textureIndex_ = 0;

	// Animation中のTimerとフラグ
	float playAnimationTimer_ = 0.0f;
	bool isPlayAnimation_ = false;
	// 逆再生
	bool reversePlayBack_ = false;
	// 点滅時間
	float blinkingTime_ = 2.0f;

};

