#pragma once

#include "JsonFunction.h"

// フィールドオブジェクト全体の項目
struct FieldObjectMainData {
	float shakePower;		// シェイクの強さ
	Vector3 shakeOffset;	// シェイクのオフセット
	float cameraShakePower;	// カメラシェイクの強さ

	float breakTimer;		// 壊れるまでの時間
	float breakEmitTimer;	// 壊れた後にエミットする時間
};

/// <summary>
/// フィールドオブジェクトの調整項目を調整、ロード、保存管理するクラス
/// </summary>
class FieldObjectAdjustItem
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
	const FieldObjectMainData& GetMainData() const { return mainData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction mainJson_;

	// フィールドオブジェクト全体の項目
	FieldObjectMainData mainData_;

};

