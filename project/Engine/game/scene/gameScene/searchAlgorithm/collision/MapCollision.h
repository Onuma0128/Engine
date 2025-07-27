#pragma once
#include <memory>
#include <vector>
#include <list>

#include "Collision2D.h"
#include "SceneJsonLoader.h"

#include "gameScene/searchAlgorithm/debug/DrawGrid.h"

// マップ1つ1つ用
struct Maptip {
	AABB_2D aabb;		// 座標格納
	Vector3 center;		// 中心座標
	bool isEnable;		// 有効かどうか
};

class MapCollision
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

	// アクセッサ
	const std::vector<std::vector<Maptip>>& GetMapData()const { return mapDatas_; }
	const float GetHalf() const { return half_; }
	const float GetCell() const { return cell_; }

private:

	// マップを作成する
	void CreateMap();
	// マップの判定を取る
	void CreateMapCollision();

	// マップを再ロードする
	void ReloadMap();

private:

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

};

