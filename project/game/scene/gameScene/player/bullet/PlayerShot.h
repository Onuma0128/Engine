#pragma once
#include <memory>
#include <vector>
#include <array>

#include "gameScene/player/bullet/PredictionObject.h"
#include "gameScene/player/bullet/PlayerBullet.h"

#include "gameScene/player/ui/PlayerBulletUI.h"
#include "gameScene/player/ui/PlayerKillCountUI.h"

class Player;

class PlayerShot
{
public:

	// 弾の初期化処理
	void Init(Player* player);
	// 弾の更新処理
	void Update();
	void UpdateUI();
	// 弾UIの描画処理
	void DrawUI();

	// 弾のリロードを管理する関数
	void ReloadBullet();
	void AllReloadBullet();
	// リロードできる弾があるか判定
	bool IsReloadBullet();

	// 弾を発射する関数
	void AttackBullet();			// 通常弾
	void SpecialAttackBullet();		// 必殺技の弾

	// キル数
	uint32_t GetNockdownCount()const { return kNockdownCount_; }

	// 弾を撃つ方向の回転
	Quaternion GetRightStickQua()const { return rightStickQuaternion_; }
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
	std::unique_ptr<PlayerKillCountUI> killCountUI_ = nullptr;
	uint32_t kNockdownCount_ = 0;

};

