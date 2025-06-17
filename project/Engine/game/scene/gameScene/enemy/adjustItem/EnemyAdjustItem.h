#pragma once

#include "JsonFunction.h"

#include "../type/EnemyType.h"

// 基本的な敵のデータ用構造体
struct EnemyTemplate {

	float speed;				// 移動速度

	float attackDistance;		// 攻撃が始まる距離
	float attackStartupTime;	// 攻撃を開始する硬直時間
	float attackActiveTime;		// 攻撃を行っている硬直時間
	float attackRecoveryTime;	// 攻撃が終わってから硬直時間
};

// 近接敵の項目
struct MeleeData {
	EnemyTemplate tempData;
};

// 遠距離敵の項目
struct RangedData {
	EnemyTemplate tempData;
};

// 盾持ち敵の項目
struct ShieldBearerData {
	EnemyTemplate tempData;
};

// 遠距離敵(強化版)の項目
struct RangedEliteData {
	EnemyTemplate tempData;
};

class EnemyAdjustItem
{
public:

	void LoadItems();

	void Editor();

	/* ============================== ゲッター ============================== */

	// プレイヤーの項目
	MeleeData GetMeleeData()const { return meleeData_; }
	// 弾の項目
	RangedData GetRangedData()const { return rangedData_; }
	// 弾の予測オブジェクトの項目
	ShieldBearerData GetShieldBearerData()const { return shieldBearerData_; }
	// リボルバーの項目
	RangedEliteData GetRangedEliteData()const { return rangedEliteData_; }

private:

	// 初期化時のパラメータをセットする
	void SetJsonParameter(JsonFunction json);
	// 敵のテンプレートデータを取ってくる
	EnemyTemplate GetTemplateData(JsonFunction json);
	void SetTemplateData(JsonFunction& json, EnemyTemplate& tempData);
	// 敵のテンプレートデータをImGuiにする
	void SetImGuiData(JsonFunction& json, EnemyTemplate& tempData);

private:

	// Jsonファイルを読み込むローダー
	JsonFunction meleeJson_;
	JsonFunction rangedJson_;
	JsonFunction shieldBearerJson_;
	JsonFunction rangedEliteJson_;

	// 近接敵の項目
	MeleeData meleeData_;
	// 遠距離敵の項目
	RangedData rangedData_;
	// 盾持ち敵の項目
	ShieldBearerData shieldBearerData_;
	// 遠距離敵の項目
	RangedEliteData rangedEliteData_;

};

