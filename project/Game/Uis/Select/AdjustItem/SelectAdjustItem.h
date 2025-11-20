#pragma once

#include "JsonFunction.h"

/// <summary>
/// セレクトの調整項目を調整、ロード、保存管理するクラス
/// </summary>
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

		float selectUiInterval;				// セレクトのUIが出てくるまでの時間
		float clearCountUiTimer_;			// 数字の出てくる時間
	};
	SelectData data_;

public:

	/// <summary>
	/// 調整項目をロードする
	/// </summary>
	void LoadItems();

	/// <summary>
	/// 調整項目を触れるようにして更新する
	/// </summary>
	void Editor();

	/// <summary>
	/// 調整項目のゲッター
	/// </summary>
	/// <returns></returns>
	const SelectData& GetSelectData()const { return data_; }

private:

	/// <summary>
	///	調整項目を保存する
	/// </summary>
	void Save();

private:

	JsonFunction json_;

};

