#pragma once

#include <vector>
#include <memory>

#include "Sprite.h"

/// <summary>
/// 数字表示UIクラス
/// </summary>
class NumbersUI : public Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Init(const Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="number"></param>
	void Update(const uint32_t number);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// サイズを設定する
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size) { transform_.size = size; }

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { transform_.position = position; }

private:
};

/// <summary>
/// プレイヤーの数を表示するUI
/// </summary>
class PlayerCountUI
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="killCount"></param>
	void Update(const uint32_t killCount);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// サイズを設定する
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size);

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="size"></param>
	void SetPosition(const Vector2& position);

	/// <summary>
	/// 数字の間隔を設定する
	/// </summary>
	/// <param name="interval"></param>
	void SetInterval(const float interval);

	/// <summary>
	/// アルファ値を設定する
	/// </summary>
	/// <param name="alpha"></param>
	void SetAlpha(const float alpha);

private:

	// 数字のSprite
	std::vector<std::unique_ptr<NumbersUI>> numbers_;

	// 間隔
	float interval_;

};

