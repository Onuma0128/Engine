#pragma once

#include "Animation.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

class BaseCharacter : public Animation, public Collider
{
public:

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

	/// <summary>
	/// ローダーを設定
	/// </summary>
	/// <param name="loader"></param>
	void SetLoader(SceneJsonLoader* loader) { loader_ = loader; }
	
protected:

	// ローダーのポインタ
	SceneJsonLoader* loader_ = nullptr;

};

