#pragma once

#include "JsonFunction.h"

// プレイヤーの項目
struct PlayerData {
	float speed;			// 移動速度
};

// 弾の項目
struct BulletData {
	float speed;			// 弾の速度
	float speed_sp;			// 必殺技の弾の速度
	Vector3 position;		// 弾が出てくる座標
};

// 弾の予測オブジェクトの項目
struct PreObjectData {
	float interval;			// オブジェクトごとの間隔
	Vector3 startPosition;	// 最初のZ座標
};

// リボルバーの項目
struct RevolverData {
	Vector3 offset;			// ペアレントして固定する座標
};

// 弾UIの項目
struct BulletUIData {
	Vector2 size;			// UIのサイズ
	Vector2 position;		// 座標(一つ一つの間隔的な)
	float startPosition;	// 最初の座標
};

class PlayerAdjustItem
{
public:

	void LoadItems();

	void Editor();

	/* ============================== ゲッター ============================== */

	// プレイヤーの項目
	PlayerData GetPlayerData()const { return playerData_; }
	// 弾の項目
	BulletData GetBulletData()const { return bulletData_; }
	// 弾の予測オブジェクトの項目
	PreObjectData GetPreObjectData()const { return preObjectData_; }
	// リボルバーの項目
	RevolverData GetRevolverData()const { return revolverData_; }
	// 弾UIの項目
	BulletUIData GetBulletUIData()const { return bulletUIData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction playerJson_;
	JsonFunction bulletJson_;
	JsonFunction preObjectJson_;
	JsonFunction revolverJson_;
	JsonFunction bulletUIJson_;

	// プレイヤーの項目
	PlayerData playerData_;
	// 弾の項目
	BulletData bulletData_;
	// 弾の予測オブジェクトの項目
	PreObjectData preObjectData_;
	// リボルバーの項目
	RevolverData revolverData_;
	// 弾UIの項目
	BulletUIData bulletUIData_;

};

