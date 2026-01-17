#pragma once

#include <memory>

#include "JsonFunction.h"
#include "Camera.h"
#include "Vector3.h"

#include "GameCamera/AdjustItem/CameraAdjustItem.h"

// 前方宣言
class Player;
class BossEnemy;
class EnemySpawnerFactory;

/// <summary>
/// ゲーム上のカメラを持つ
/// </summary>
class GameCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// カメラの更新処理
	/// </summary>
	void Update();
	void SabUpdate(const Vector3& shakeOffset);
	void BossUpdate(const Vector3& shakeOffset);

	/// <summary>
	/// カメラシェイクさせるセッター
	/// </summary>
	/// <param name="shakeStrength"></シェイクする最大値>
	void SetShake(float shakeStrength) { shakeStrength_ = shakeStrength; }

	/// <summary>
	/// ボスのカメラをリセットする
	/// </summary>
	void BossCameraReset();
	void BossCameraEnd();

	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }
	void SetSpawner(EnemySpawnerFactory* spawner) { spawner_ = spawner; }
	Camera* GetCamera() { return mainCamera_.get(); }

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	bool playerIsAlive_ = true;

	// ボスのポインタ
	BossEnemy* boss_ = nullptr;

	// 敵スポナーファクトリー
	EnemySpawnerFactory* spawner_ = nullptr;

	// カメラの調整項目
	std::unique_ptr<CameraAdjustItem> items_ = nullptr;

	// カメラ
	std::shared_ptr<Camera> mainCamera_ = nullptr;
	std::shared_ptr<Camera> sabCamera_ = nullptr;
	std::shared_ptr<Camera> bossCamera_ = nullptr;

	Vector3 mainCameraAddPos_ = {};
	float shakeStrength_ = 0.0f;
	float shakeDecay_ = 0.9f;

	// サブカメラのアニメーション用
	struct SabCameraAnimation {
		Vector3 sabCameraOffset = {};
		bool isRotate = false;
		float rotateTimer = 0.0f;
		float rotateSpeed = 1.0f;
		float radius = 10.0f;
		float positionY;
	};
	SabCameraAnimation sabAnima_;

	// ボスカメラのステート
	enum class BossCameraState {
		Startup,		// カメラ切り替えまでの時間
		Active,			// カメラが動いている時間
		Recover,		// カメラが切り替わるまでの時間
		End,			// 終了
	};
	BossCameraState bossCameraState_ = BossCameraState::Startup;
	// ボスカメラのタイム
	float bossCameraTime_ = -1.0f;
	// 前のカメラ座標
	Vector3 preBossCameraPosition_{};

};