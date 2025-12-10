#pragma once

#include <memory>
#include <string>

#include "Sprite.h"
#include "JsonFunction.h"

/// <summary>
/// ベースUIクラス
/// </summary>
class BaseUI
{
private:

	/// <summary>
	/// UIの調整項目パラメータ
	/// </summary>
	struct UiParameters
	{
		std::string filename;			// 保存ファイル名
		std::string biginName;			// ImGuiの入れる名前
		std::string texture;			// テクスチャ名
		std::string noiseTexture;		// ノイズテクスチャ名

		Vector2 anchorPoint;			// アンカーポイント
		Transform2D transform;			// トランスフォーム
		Vector4 color;					// カラー
		float edgeWidth;				// エッジ幅	
		Vector3 noiseColor;				// ノイズカラー

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

	/// <summary>
	///	初期化
	/// </summary>
	/// <param name="uiName"></このUIの名前を設定する>
	/// <param name="biginName"></ImGuiで描画するBiginNameを設定する>
	void Init(const std::string uiName, const std::string biginName, bool isNoiseTexture = false);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ImGuiを描画
	/// </summary>
	void DrawImGui();

	/// <summary>
	/// フェードイン
	/// </summary>
	void FadeIn();
	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOut();

	/// <summary>
	/// 点滅させる
	/// </summary>
	void Blinking();
	/// <summary>
	/// 点滅リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// スプライトを取得
	/// </summary>
	/// <returns></returns>
	Sprite* GetSprite()const { return ui_.get(); }

	/// <summary>
	/// アニメーション中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsPlayAnimation()const { return isPlayAnimation_; }

private:

	/// <summary>
	/// UIアニメーション
	/// </summary>
	void UI_Animation();

	/// <summary>
	/// 調整項目を保存
	/// </summary>
	void Save();

private:

	std::unique_ptr<Sprite> ui_ = nullptr;

	UiParameters parameters_;

	JsonFunction json_;

	// 選択中インデックス
	uint32_t textureIndex_ = 0;
	uint32_t noiseTextureIndex_ = 0;

	// Animation中のTimerとフラグ
	float playAnimationTimer_ = 0.0f;
	bool isPlayAnimation_ = false;
	// Noiseテクスチャを使うか
	bool isNoiseTexture_ = false;
	// 逆再生
	bool reversePlayBack_ = false;
	// 点滅時間
	float blinkingTime_ = 2.0f;

};

