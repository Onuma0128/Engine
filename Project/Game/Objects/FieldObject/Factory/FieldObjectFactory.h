#pragma once

#include <memory>
#include <list>

#include "SceneJsonLoader.h"
#include "objects/fieldObject/FieldObject.h"
#include "objects/fieldObject/adjustItem/FieldObjectAdjustItem.h"

/// <summary>
/// フィールドオブジェクトを管理するクラス
/// </summary>
class FieldObjectFactory
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="loader"></Jsonの保存データを取得する>
	void Init(SceneJsonLoader loader);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

	std::list<std::unique_ptr<FieldObject>> fieldObjects_;

	std::unique_ptr<FieldObjectAdjustItem> items_ = nullptr;

};

