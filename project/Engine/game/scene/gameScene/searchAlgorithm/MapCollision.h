#pragma once
#include <memory>
#include <vector>
#include <list>

#include "Collision2D.h"
#include "SceneJsonLoader.h"

#include "DrawGrid.h"

// マップ1つ1つ用
struct Maptip {
	AABB_2D aabb;		// 座標格納
	Vector3 center;		// 中心座標
	bool isEnable;		// 有効かどうか
};

struct GridPos { uint32_t x, z; };

class MapCollision
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

	// 座標を入れたら配列番号に変換する関数
	uint32_t ArrayIndex_X(const float x) const;
	uint32_t ArrayIndex_Z(const float z) const;

	// アクセッサ
	const std::vector<std::vector<Maptip>>& GetMapData()const { return mapDatas_; }

private:

	float cell_;		// グリッドの1マスの幅
	float size_;		// グリッドの全体サイズ
	float half_;		// グリッドの半径

	// マップの座標を格納する多重配列
	std::vector<std::vector<Maptip>> mapDatas_;

	// グリッドを表示
	std::unique_ptr<DrawGrid> grid_ = nullptr;

};

