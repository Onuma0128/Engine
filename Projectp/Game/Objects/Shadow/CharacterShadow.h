#pragma once

#include "Object3d.h"

/// <summary>
/// キャラクターの影を表すクラス
/// </summary>
class CharacterShadow : public Object3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="transform"></param>
	void Init(const WorldTransform& transform);
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画フラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetDraw(bool flag) { Object3d::GetMaterial().enableDraw = flag; }

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) { transform_.translation_ = position; }
};

