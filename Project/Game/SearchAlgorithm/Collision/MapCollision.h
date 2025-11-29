#pragma once
#include <memory>
#include <vector>
#include <list>

#include "Collision2D.h"
#include "SceneJsonLoader.h"

#include "searchAlgorithm/debugGrid/DrawGrid.h"

/// <summary>
/// マップの情報
/// </summary>
struct Maptip {
	AABB_2D aabb;		// 座標格納
	Vector3 center;		// 中心座標
	bool isEnable;		// 有効かどうか
};

/// <summary>
/// マップの衝突判定を管理するクラス
/// </summary>
class MapCollision
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="object"></Jsonの保存データを取得する>
	void Init(SceneJsonLoader loader);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// マップデータを取得する
	/// </summary>
	/// <returns></returns>
	const std::vector<std::vector<Maptip>>& GetMapData()const { return mapDatas_; }

	/// <summary>
	/// グリッドの半径を取得する
	/// </summary>
	/// <returns></returns>
	const float GetHalf() const { return half_; }

	/// <summary>
	/// グリッドの1マスの幅を取得する
	/// </summary>
	/// <returns></returns>
	const float GetCell() const { return cell_; }

	/// <summary>
	/// 座標や回転が変わったObjectの判定を再計算する
	/// </summary>
	/// <param name="prevOBB"></前のOBB>
	/// <param name="currentOBB"></現在のOBB>
	void ReTargetMapCollisionOBB(const OBB_2D& prevOBB, const OBB_2D& currentOBB);

private:

	/// <summary>
	/// マップを作成する
	/// </summary>
	void CreateMap();
	
	/// <summary>
	/// マップの衝突判定を作成する
	/// </summary>
	void CreateMapCollision();

	/// <summary>
	/// マップを再ロードする
	/// </summary>
	void ReloadMap();

private:

	const float kHalfValue_ = 0.5f;
	float cell_;		// グリッドの1マスの幅
	float size_;		// グリッドの全体サイズ
	float half_;		// グリッドの半径

	// マップの座標を格納する多重配列
	std::vector<std::vector<Maptip>> mapDatas_;
	// OBBの判定が付いてる用
	std::list<OBB_2D> objects_obb_;
	// Sphereの判定が付いている用
	std::list<Circle> objects_circle_;

	// グリッドを表示
	std::unique_ptr<DrawGrid> grid_ = nullptr;
	bool debugLine_ = false;

};

