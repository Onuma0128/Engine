#pragma once

#include "objects/enemy/weapon/EnemyWeaponBase.h"

/// <summary>
/// 斧のクラス
/// </summary>
class EnemyAxe : public EnemyWeaponBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyAxe(BaseEnemy* enemy);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type"></コライダーのタイプ>
	/// <param name="name"></コライダーの名前>
	void Init(ColliderType type, const std::string& name) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 当たり判定の処理
	/// </summary>
	/// <param name="other"></当たったColliderのポインタを取得する>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	/// <summary>
	/// アクティブ状態を設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetIsActive(bool flag) override;

	/// <summary>
	/// アクティブ状態を取得する
	/// </summary>
	/// <returns></returns>
	bool GetIsActive()override;
};

