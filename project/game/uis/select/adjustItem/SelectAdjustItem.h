#pragma once

#include "JsonFunction.h"

class SelectAdjustItem
{
private:

	// セレクトの調整項目
	struct SelectData {

		float killNumberUiInterval;			// キル数の間隔
		Vector2 killNumberUiSize;			// キル数のサイズ
		Vector2 killNumberUiPos;			// キル数の座標
		
		float hitRateNumberUiInterval;		// 命中率の間隔
		Vector2 hitRateNumberUiSize;		// 命中率のサイズ
		Vector2 hitRateNumberUiPos;			// 命中率の座標

		float clearCountUiTimer_;			// 数字の出てくる時間
	};
	SelectData data_;

public:

	// Json保存
	void Init();

	void Editor();

	const SelectData& GetSelectData()const { return data_; }

private:

	void Save();

private:

	JsonFunction json_;

};

