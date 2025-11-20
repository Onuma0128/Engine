#pragma once
#include <memory>
#include <vector>
#include <list>

#include "WorldTransform.h"
#include "Collider.h"
#include "Sprite.h"

/// <summary>
/// プレイヤーのレティクルを管理するクラス
/// </summary>
class PlayerReticle :public Sprite,Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="isPlayingMouse"></param>
	void Update(bool isPlayingMouse);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	/// <summary>
	/// コライダーのアクティブ状態を設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetColliderActive(bool flag) { Collider::isActive_ = flag; }

	/// <summary>
	/// 敵のコライダーリストを取得する
	/// </summary>
	/// <returns></returns>
	const std::list<Collider*>& GetEnemyColliders()const { return enemyColliders_; }

	/// <summary>
	/// 敵のコライダーを消していく
	/// </summary>
	void EnemyCollidersPopBack() { enemyColliders_.pop_back(); }

	/// <summary>
	/// 敵のコライダーを全て消す
	/// </summary>
	void EnemyCollidersClear() { enemyColliders_.clear(); }

	/// <summary>
	/// ヒットカウントをリセットする
	/// </summary>
	void ResetHitCount() { 
		hitCount_ = 0;
		Collider::isActive_ = false;
	}

private:

	/// <summary>
	/// 当たり判定用セグメントの更新
	/// </summary>
	void SegmentUpdate();

private:

	// 敵のColliderを取得用
	std::list<Collider*> enemyColliders_;
	// 何体敵と当たったかカウント用
	uint32_t hitCount_ = 0;
	// ヒット時のcolor値
	float reticleColorTimer_ = 0.0f;

};

/// <summary>
/// プレイヤーのレイ用レティクルクラス
/// </summary>
class PlayerRayReticle : public Sprite 
{
	public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// レイヒット状態を設定
	/// </summary>
	/// <param name="flag"></param>
	void SetRaticleAlpha(bool flag);

	/// <summary>
	/// ワールド座標をスクリーン座標に変換して設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position);

private:

	// レイのalpha時間
	float alphaTimer_ = 0.0f;

};

