#pragma once

#include <memory>
#include <list>

#include "SceneJsonLoader.h"
#include "objects/fieldObject/FieldObject.h"
#include "objects/fieldObject/adjustItem/FieldObjectAdjustItem.h"

/// 前方宣言
class MapCollision;
class GameCamera;

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

	/// <summary>
	/// マップの衝突判定を取得する
	/// </summary>
	/// <param name="mapCollision"></param>
	void SetMapCollision(MapCollision* mapCollision) { mapCollision_ = mapCollision; }

	/// <summary>
	/// カメラを取得する
	/// </summary>
	/// <param name="gameCamera"></param>
	void SetGameCamera(GameCamera* gameCamera) { gameCamera_ = gameCamera; }

private:

	std::list<std::unique_ptr<FieldObject>> fieldObjects_;

	std::unique_ptr<FieldObjectAdjustItem> items_ = nullptr;

	MapCollision* mapCollision_ = nullptr;
	GameCamera* gameCamera_ = nullptr;

};

