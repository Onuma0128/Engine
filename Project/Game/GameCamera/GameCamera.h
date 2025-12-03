#pragma once

#include <memory>

#include "JsonFunction.h"
#include "Camera.h"
#include "Vector3.h"

class Player;

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
	/// JSON保存による調整項目の初期化とロード、保存
	/// </summary>
	void JsonInit();
	void SaveJson();
	void ValueImGui();

	/// <summary>
	/// カメラの更新処理
	/// </summary>
	void Update();
	void SabUpdate(const Vector3& shakeOffset);

	/// <summary>
	/// カメラシェイクさせるセッター
	/// </summary>
	/// <param name="shakeStrength"></シェイクする最大値>
	void SetShake(float shakeStrength) { shakeStrength_ = shakeStrength; }

	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	Camera* GetCamera() { return mainCamera_.get(); }

private:

	Player* player_ = nullptr;
	bool playerIsAlive_ = true;

	std::shared_ptr<Camera> mainCamera_ = nullptr;
	std::shared_ptr<Camera> sabCamera_ = nullptr;

	Vector3 mainCameraAddPos_ = {};

	float shakeStrength_ = 0.0f;
	float shakeDecay_ = 0.9f;

	struct SabCameraAnimation {
		Vector3 sabCameraOffset = {};
		bool isRotate = false;
		float rotateTimer = 0.0f;
		float rotateSpeed = 1.0f;
		float radius = 10.0f;
		float positionY;
	};
	SabCameraAnimation sabAnima_;

	JsonFunction data_;

};