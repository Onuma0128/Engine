#pragma once

#include "JsonFunction.h"

/// <summary>
/// ゲームカメラのデータ
/// </summary>
struct GameCameraData {
	Vector3 mainRotate;				// メインカメラの回転
	Vector3 mainPosition;			// メインカメラの座標

	Vector3 sabPosition;			// サブカメラの座標
	bool isSabRotate;				// サブカメラが回転するか
	float sabRotateSpeed;			// サブカメラの回転スピード
	float sabRadius;				// サブカメラの回転する半径
	float sabPosY;					// サブカメラの高さ

	float bossStartupTime;			// ボスカメラに移行までの待機時間
	float bossActiveTime;			// ボスカメラの動いている時間
	float bossRecoverTime;			// ボスカメラの終了後の待機時間
	Vector3 bossStartRotate;		// ボスカメラの最初の回転
	Vector3 bossStartPosition;		// ボスカメラの最初の座標
	Vector3 bossEndRotate;			// ボスカメラの最後の回転
	Vector3 bossEndPosition;		// ボスカメラの最後の座標
};

/// <summary>
/// カメラの調整項目を調整、ロード、保存管理するクラス
/// </summary>
class CameraAdjustItem
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
	const GameCameraData& GetCameraData() { return cameraData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction cameraJson_;


	GameCameraData cameraData_;

};

