#pragma once

#include "Animation.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

/// <summary>
/// ベースキャラクタークラス
/// </summary>
class BaseCharacter : public NumaEngine::Animation, public NumaEngine::Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

};

