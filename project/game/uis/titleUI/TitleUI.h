#pragma once

#include "BaseUI.h"

/// <summary>
/// タイトルUIクラス
/// </summary>
class TitleUI : public BaseUI
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

};

