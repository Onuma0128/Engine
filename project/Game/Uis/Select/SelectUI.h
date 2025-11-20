#pragma once

#include "BaseUI.h"

/// <summary>
/// セレクトUIクラス
/// </summary>
class SelectUI : public BaseUI
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="name"></param>
	void Init(const std::string& name);

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:

};

