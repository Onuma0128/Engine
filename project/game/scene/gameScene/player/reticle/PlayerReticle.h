#pragma once
#include <memory>
#include <vector>
#include <list>

#include "WorldTransform.h"
#include "Collider.h"
#include "Sprite.h"

class PlayerReticle :public Sprite,Collider
{
public:

	void Init();

	void GlobalInit();

	void Update(bool isPlayingMouse);

	void Draw();

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	void SetColliderActive(bool flag) { Collider::isActive_ = flag; }

	const std::list<Collider*>& GetEnemyColliders()const { return enemyColliders_; }
	void EnemyCollidersPopBack() { enemyColliders_.pop_back(); }
	void EnemyCollidersClear() { enemyColliders_.clear(); }
	void ResetHitCount() { 
		hitCount_ = 0;
		Collider::isActive_ = false;
	}

private:

	void SegmentUpdate();

private:

	// 敵のColliderを取得用
	std::list<Collider*> enemyColliders_;
	// 何体敵と当たったかカウント用
	uint32_t hitCount_ = 0;
	// ヒット時のcolor値
	float reticleColorTimer_ = 0.0f;

};

