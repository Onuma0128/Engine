#pragma once
#include <string>

#include "Object3d.h"
#include "Collider.h"

class BaseEnemy;

/// <summary>
/// 敵の武器の基底クラス
/// </summary>
class EnemyWeaponBase : public Object3d, public Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyWeaponBase() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyWeaponBase(BaseEnemy* enemy);

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type"></コライダーのタイプ>
	/// <param name="name"></コライダーの名前>
	virtual void Init(ColliderType type, const std::string& name) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// アクティブ状態を設定する
	/// </summary>
	/// <param name="flag"></param>
	virtual void SetIsActive(bool flag) = 0;

	/// <summary>
	/// アクティブ状態を取得する
	/// </summary>
	/// <returns></returns>
	virtual bool GetIsActive() = 0;

protected:

	BaseEnemy* enemy_ = nullptr;

};

