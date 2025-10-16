#pragma once
#include <memory>
#include <vector>

#include "Line3d.h"
#include "Collision2D.h"

class DrawGrid
{
public:

	void Init(const float cell, const float size);

	void Update();

	void SetDrawGrid(bool flag);

	// フィールドのグリッド用
	void SetGridPositions(const float cell, const float size);

	// フィールドの当たっているグリッド用
	void HitGridInit();
	void HitAABB(const AABB_2D& aabb);
	void HitAABBClear() { hitGridPositions_.clear(); }
	void SetHitAABBPositions() { hitGrid_->SetPositions(hitGridPositions_); }

private:

	// グリッド
	std::unique_ptr<Line3d> grid_ = nullptr;

	// 判定が取れた所だけ赤くする
	std::unique_ptr<Line3d> hitGrid_ = nullptr;
	std::vector<Vector3> hitGridPositions_;

};

