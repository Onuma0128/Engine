#pragma once

#include "Object3d.h"

class PlayerBullet : public Object3d
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

	// リロードをする関数
	void Reload();
	// 攻撃をする関数
	void Attack(const WorldTransform& transform);

	bool GetIsActive()const { return isActive_; }
	bool GetIsReload()const { return isReload_; }

private:

	// 今動いているか
	bool isActive_ = false;
	// 動いている時間
	float activeFrame_ = 0.0f;
	// リロードされているか
	bool isReload_ = true;


	Vector3 velocity_;

};

