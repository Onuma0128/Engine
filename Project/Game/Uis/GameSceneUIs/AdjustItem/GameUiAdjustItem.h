#pragma once

#include "JsonFunction.h"

// セレクトの調整項目
struct GameUiData {

	float killNumberUiInterval;			// キル数の間隔
	Vector2 killNumberUiSize;			// キル数のサイズ
	Vector2 killNumberUiPos;			// キル数の座標

	int maxKillCount;
	float maxKillNumberUiInterval;		// キル数の間隔
	Vector2 maxKillNumberUiSize;		// キル数のサイズ
	Vector2 maxKillNumberUiPos;			// キル数の座標

	float startInTime;					// カメラ切り替え前のフェードイン
	float startOutTime;					// カメラ切り替え後のフェードアウト
	float activeTime;					// ボス登場演出中
	float recoverInTime;				// カメラ切り替え後のフェードイン
	float recoverOutTime;				// カメラ切り替え前のフェードアウト
};

/// <summary>
/// ゲームシーンUIの調整項目を調整、ロード、保存管理するクラス
/// </summary>
class GameUiAdjustItem
{
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
	const GameUiData& GetSelectData()const { return data_; }

private:

	/// <summary>
	///	調整項目を保存する
	/// </summary>
	void Save();

private:

	JsonFunction json_;

	GameUiData data_;

};

