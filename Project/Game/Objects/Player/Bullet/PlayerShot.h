#pragma once
#include <memory>
#include <vector>
#include <array>

#include "Collider.h"

#include "objects/player/bullet/PredictionObject.h"
#include "objects/player/bullet/PlayerBullet.h"
#include "objects/player/reticle/PlayerReticle.h"

#include "uis/player/PlayerBulletUI.h"
#include "uis/player/PlayerCountUI.h"

class Player;

/// <summary>
/// プレイヤーの弾を管理するクラス
/// </summary>
class PlayerShot : public Collider
{
public:

	/// <summary>
	/// プレイヤーの弾を初期化する
	/// </summary>
	/// <param name="player"></param>
	void Init(Player* player);

	/// <summary>
	/// 弾の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 弾のUIを更新
	/// </summary>
	void UpdateUI();

	/// <summary>
	/// 弾UIの描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 弾のリロードを行う
	/// </summary>
	void ReloadBullet();

	/// <summary>
	/// 全ての弾のリロードを行う
	/// </summary>
	void AllReloadBullet();

	/// <summary>
	/// リロードできる弾があるか判定
	/// </summary>
	bool IsReloadBullet();

	/// <summary>
	/// 弾を発射する関数
	/// </summary>
	void AttackBullet();			// 通常弾
	void SpecialAttackBullet();		// 必殺技の弾

	/// <summary>
	/// キル数を1増やす
	/// </summary>
	void AddNockdownCount() { ++kNockdownCount_; }
	void AddChargeCount() { ++kChargeCount_; }

	void ResetChargeCount() { kChargeCount_ = 0; }

	/// <summary>
	/// キル数を取得する
	/// </summary>
	/// <returns></returns>
	uint32_t GetNockdownCount()const { return kNockdownCount_; }
	uint32_t GetChargeCount()const { return kChargeCount_; }
	
	/// <summary>
	/// 命中率を取得する
	/// </summary>
	/// <returns></returns>
	uint32_t GetHitRate()const { 
		if (kHitRate_ == 0) { return 0; }
		float result = static_cast<float>(kNockdownCount_) / static_cast<float>(kHitRate_);
		return static_cast<uint32_t>(result * 100.0f);
	}

	/// <summary>
	/// 弾を撃つ方向の回転を取得する
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRightStickQua()const { return rightStickQuaternion_; }

	/// <summary>
	/// 弾を撃つ方向の回転を設定する
	/// </summary>
	/// <param name="q"></param>
	void SetRightStickQua(const Quaternion& q) { rightStickQuaternion_ = q; }

	/// <summary>
	/// Rayが当たったかどうかを取得する
	/// </summary>
	/// <returns></returns>
	const bool GetIsRayHit()const { return isRayHit_; }

	/// <summary>
	/// Rayが当たった座標を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRayHitPosition()const { return rayHitPosition_; }

	/// <summary>
	/// Rayが当たったかどうかをリセットする
	/// </summary>
	void ResetRayHit() { 
		if (isRayHit_) { targetPosition_ = rayHitPosition_; }
		else { targetPosition_ = Vector3::ExprZero; }
		isRayHit_ = false; 
		rayHitPosition_ = Vector3::ExprZero;
	}

	/// <summary>
	/// 攻撃をしたかどうかを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetIsShot(const bool flag) { isShot_ = flag; }
	const bool GetIsShot()const { return isShot_; }

	/// <summary>
	/// 攻撃ができたかどうかを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetIsCanAttack(const bool flag) { isCanAttack_ = flag; }
	const bool GetIsCanAttack()const { return isCanAttack_; }

	/// <summary>
	/// 集合要求を設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetGatherRequested(const bool flag) { isGatherRequested_ = flag; }
	bool GetGatherRequested()const { return isGatherRequested_; }

	/// <summary>
	/// Rayが当たった座標を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetTargetPosition()const { return targetPosition_; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionStay(Collider* other) override;

	/// <summary>
	/// Rayの更新
	/// </summary>
	void RayUpdate();

private:

	Player* player_ = nullptr;

	// 攻撃したかどうか
	bool isShot_ = false;
	// 攻撃ができたか
	bool isCanAttack_ = false;
	Vector3 targetPosition_{};
	// 集合要求したかどうか
	bool isGatherRequested_ = false;

	// Rayのスプライト
	std::unique_ptr<PlayerRayReticle> rayReticle_ = nullptr;
	Vector3 rayDirection_ = Vector3::ExprUnitZ;
	// Rayが当たったかどうか
	bool isRayHit_ = false;
	// Rayが当たった座標
	Vector3 rayHitPosition_{};

	// 弾を撃つ方向の回転
	Quaternion rightStickQuaternion_{};

	// 予測の点のオブジェクト
	std::array<std::unique_ptr<PredictionObject>, 3> predictionObjects_;

	// 弾を6つ生成する
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;
	std::vector<std::unique_ptr<PlayerBulletUI>> bulletUIs_;
	uint32_t kBulletCount_ = 0;

	// 必殺技用の弾を6つ生成
	std::vector<std::unique_ptr<PlayerBullet>> specialBullets_;

	// 敵のキル数UI
	std::unique_ptr<PlayerCountUI> killCountUI_ = nullptr;
	uint32_t kNockdownCount_ = 0;
	uint32_t kChargeCount_ = 0;

	// 撃った弾数を取得する
	uint32_t kHitRate_ = 0;

};

