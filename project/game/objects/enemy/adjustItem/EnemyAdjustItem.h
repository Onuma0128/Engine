#pragma once
#include <array>

#include "JsonFunction.h"

// 基本的な敵のデータ用構造体
struct EnemyTemplate {

	float speed;				// 移動速度

	float attackDistance;		// 攻撃が始まる距離
	float attackStartupTime;	// 攻撃を開始する硬直時間
	float attackActiveTime;		// 攻撃を行っている硬直時間
	float attackRecoveryTime;	// 攻撃が終わってから硬直時間
	float attackCoolTime;		// 攻撃が終了した後、次の攻撃までのクールタイム
};

// 敵全体の項目
struct EnemyMainData {
	bool debugIsSpawn;			// デバッグ中のみ使えるスポーンするかしないか
	bool nowSpawn;				// 今スポーンをさせる
	uint32_t spawnIndex;		// スポーンさせる時のIndex

	int maxSpawn;				// 敵が最大マップに何体まで出せるか上限
	int nextWaveKillCount;		// 敵を何体倒したら強い敵が出てくるかカウント

	Vector3 colliderSize;		// 敵本体のコライダーサイズ
	Vector3 colliderOffset;		// 敵本体のコライダー座標

	float margin;				// 敵のアニメーションを判定しない余白距離

	float searchUpdateTime;		// 敵の探索を更新する時間
	bool debugSpline;			// 敵の探索経路デバッグ用
};

// 近接敵の項目
struct MeleeData {
	EnemyTemplate tempData;
	float colliderSize;			// コライダーのサイズ
	Vector3 colliderOffset;		// コライダーの座標(敵ペアレント時の)
};

// 遠距離敵の項目
struct RangedData {
	EnemyTemplate tempData;
	float bulletSpeed;			// 撃つ弾の速度
	Vector3 colliderSize;		// コライダーのサイズ
	Vector3 colliderOffset;		// コライダーの座標
	Vector3 planeSize;			// プレーンエフェクトのサイズ
	Vector3 planeOffset;		// プレーンエフェクトの座標
};

// 盾持ち敵の項目
struct ShieldBearerData {
	EnemyTemplate tempData;
	float attackColliderSize;			// コライダーのサイズ
	Vector3 attackColliderOffset;		// コライダーの座標(敵ペアレント時の)

	float lerpSpeed;					// 盾が回転するラープ速度
	Vector3 shieldSize;					// 盾のサイズ
	Vector3 shieldOffset;				// 盾の座標(敵ペアレント時の)
	Vector3 shieldColliderSize;			// 盾コライダーのサイズ
	Vector3 shieldColliderOffset;		// 盾コライダーの座標(敵ペアレント時の)
};

// 遠距離敵(強化版)の項目
struct RangedEliteData {
	EnemyTemplate tempData;
	float bulletSpeed;					// 撃つ弾の速度
	float bulletRadSpace;				// 弾同士の間隔の角度
	std::array<Vector3, 3> planeSize;	// プレーンエフェクトのサイズ
	std::array<Vector3, 3> planeOffset;	// プレーンエフェクトの座標
};

class EnemyAdjustItem
{
public:

	void LoadItems();

	void Editor();

	/* ============================== ゲッター ============================== */

	// 敵全体の項目
	EnemyMainData GetMainData()const { return mainData_; }
	// 近接敵の項目
	MeleeData GetMeleeData()const { return meleeData_; }
	// 遠距離敵の項目
	RangedData GetRangedData()const { return rangedData_; }
	// 盾持ち敵の項目
	ShieldBearerData GetShieldBearerData()const { return shieldBearerData_; }
	// 遠距離敵(強化版)の項目
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
	JsonFunction mainJson_;
	JsonFunction meleeJson_;
	JsonFunction rangedJson_;
	JsonFunction shieldBearerJson_;
	JsonFunction rangedEliteJson_;

	// 敵全体の項目
	EnemyMainData mainData_;
	// 近接敵の項目
	MeleeData meleeData_;
	// 遠距離敵の項目
	RangedData rangedData_;
	// 盾持ち敵の項目
	ShieldBearerData shieldBearerData_;
	// 遠距離敵の項目
	RangedEliteData rangedEliteData_;

};

