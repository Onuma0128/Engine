#pragma once
#include <memory>
#include <vector>
#include <array>

#include "objects/player/bullet/PredictionObject.h"
#include "objects/player/bullet/PlayerBullet.h"

#include "uis/player/PlayerBulletUI.h"
#include "uis/player/PlayerCountUI.h"

class Player;

/// <summary>
/// プレイヤーの弾を管理するクラス
/// </summary>
class PlayerShot
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
	/// キル数を取得する
	/// </summary>
	/// <returns></returns>
	uint32_t GetNockdownCount()const { return kNockdownCount_; }
	
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
	Quaternion GetRightStickQua()const { return rightStickQuaternion_; }

	/// <summary>
	/// 弾を撃つ方向の回転を設定する
	/// </summary>
	/// <param name="q"></param>
	void SetRightStickQua(const Quaternion& q) { rightStickQuaternion_ = q; }

private:

	Player* player_ = nullptr;

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
	// 撃った弾数を取得する
	uint32_t kHitRate_ = 0;

};

