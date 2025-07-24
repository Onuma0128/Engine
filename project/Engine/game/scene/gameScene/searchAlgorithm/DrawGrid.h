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

	void HitGridInit();
	void HitAABB(const AABB_2D& aabb);

private:

	// グリッド
	std::unique_ptr<Line3d> grid_ = nullptr;

	// 判定が取れた所だけ赤くする
	std::unique_ptr<Line3d> hitGrid_ = nullptr;
	std::vector<Vector3> hitGridPositions_;

};

