#pragma once

#include "Object3d.h"

class Player;

/// <summary>
/// 弾の予測オブジェクトクラス
/// </summary>
class PredictionObject :public Object3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="offset"></param>
	void Update(const Vector3& offset);

	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;
	
};
