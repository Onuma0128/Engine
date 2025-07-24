#pragma once
#include <memory>
#include <vector>

#define NOMINMAX

#include "Vector2.h"
#include "Vector3.h"

#include "MapCollision.h"

class PathFinder
{
public:

	bool Search(const Vector3& startW, const Vector3& goalW, std::vector<Vector3>& outputPositions);

	void SetMapData(std::vector<std::vector<Maptip>> mapDatas) { mapDatas_ = mapDatas; }

private:

	// 再帰ノード
	struct A_StarNode {
		GridPos pos;				// 座標(グリッドのIndex番号)
		uint32_t actualCost;		// 実コスト
		uint32_t estimatedCost;		// 推定コスト
		uint32_t score;				// スコア(実コスト + 推定コスト)
		A_StarNode* parent;			// 親のノード
	};

	std::vector<std::vector<Maptip>> mapDatas_;
};

