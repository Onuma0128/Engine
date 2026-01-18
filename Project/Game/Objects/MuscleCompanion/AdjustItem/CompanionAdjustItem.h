#pragma once

#include "JsonFunction.h"

// 仲間全体の調整項目
struct CompanionMainData {
	int maxHP;						// 最大体力
	float speed;					// 移動速度
	float hitPushSpeed;				// 仲間同士の押し出しスピード
	float objectScale;				// 仲間のオブジェクトスケール
	float colliderSize;				// 仲間本体のコライダーサイズ
	Vector3 colliderOffset;			// 仲間本体のコライダー座標

	int blinkingHP;					// 点滅し始める体力
	float blinkingColor;			// 点滅する振れ幅
	float blinkingTimeScale;		// 点滅する時間倍率

	float distanceToAlly;			// 仲間同士の距離
	float followerColliderSize;		// 後続判定された仲間のコライダーサイズ

	float searchUpdateTime;			// 仲間の探索を更新する時間
	float searchCancelDistance;		// 仲間の探索をやめる距離
	bool debugSpline;				// 仲間の探索経路デバッグ用

	float clearStateTime;			// 仲間がクリアステートになる時間
};

// 仲間のダッシュの調整項目
struct CompanionDashData {
	float dashSpeed;				// 攻撃時の移動速度
	float dashTargetDistance;		// ダッシュ時の目指す距離
	float dashColliderScale;		// 攻撃時のコライダースケール倍率
	float searchDashColliderScale;	// 集合の戻り時の攻撃コライダースケール倍率

	float pushUpTime;				// 待ち時間の後の攻撃力アップする時間
};

// 仲間の攻撃の調整項目
struct CompanionAttackData {
	float shakePowerHigh;			// カメラシェイクの振動値
	float shakePowerLow;			// カメラシェイクの振動値
	float attackColliderSize;		// 攻撃時のコライダーサイズ
	Vector3 attackColliderOffset;	// 攻撃時のコライダー座標

	float firstDashTime;			// 最初のダッシュした後の硬直時間
	float attackStartupTime;		// 攻撃を開始する硬直時間
	float attackActiveTime;			// 攻撃を行っている硬直時間
	float attackRecoveryTime;		// 攻撃が終わってから硬直時間
};

// 仲間のノックバックの調整項目
struct CompanionKnockbackData {
	float knockbackSpeed;			// ノックバックスピード
	float knockbackTime;			// ノックバックしている時間

	Vector3 shieldKnockbackDire;	// ノックバックする方向
	float shieldKnockbackSpeed;		// ノックバックスピード
	float shieldKnockbackTime;		// ノックバックしている時間
};

// 仲間のエフェクトの調整項目
struct CompanionEffectData {
	Vector3 nextArrowScale;				// 次発射される矢印のスケール
	Vector3 nextArrowPosition;			// 次発射される矢印の座標
	float nextArrowAnimaTime;			// 矢印のアニメーション時間
	Vector3 nextArrowVarianceScale;		// 次発射される矢印の変動スケール
	Vector3 nextArrowVariancePosition;	// 次発射される矢印の変動座標

	float lerpSpeed;					// エフェクトのラープ移動スピード
};

// 仲間の効果音の音量
struct CompanionSeVolumeData {
	float dashHit;					// ダッシュで敵に当たった時
	float footsteps;				// 足音
	float getDamage;				// ダメージを受けた時
	float giveDamage;				// ダメージを与えた時
	float set;						// 集合
	float shot;						// 発射
};

/// <summary>
/// 仲間の調整項目を調整、ロード、保存管理するクラス
/// </summary>
class CompanionAdjustItem
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
	const CompanionMainData& GetMainData() const { return mainData_; }
	const CompanionDashData& GetDashData() const { return dashData_; }
	const CompanionAttackData& GetAttackData() const { return attackData_; }
	const CompanionKnockbackData& GetKnockbackData() const { return knockbackData_; }
	const CompanionEffectData& GetEffectData() const { return effectData_; }
	const CompanionSeVolumeData& GetSeVolumeData() const { return seVolumeData_; }

private:

	// Jsonファイルを読み込むローダー
	JsonFunction mainJson_;
	JsonFunction dashJson_;
	JsonFunction attackJson_;
	JsonFunction knockbackJson_;
	JsonFunction effectJson_;
	JsonFunction seVolumeJson_;

	// 仲間全体の項目
	CompanionMainData mainData_;
	// 仲間のダッシュの項目
	CompanionDashData dashData_;
	// 仲間の攻撃の項目
	CompanionAttackData attackData_;
	// 仲間のノックバックの項目
	CompanionKnockbackData knockbackData_;
	// 仲間のエフェクトの項目
	CompanionEffectData effectData_;
	// 仲間の効果音の項目
	CompanionSeVolumeData seVolumeData_;

};

