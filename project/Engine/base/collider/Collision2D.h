#pragma once

#include "Vector2.h"

// 円
struct Circle {
	Vector2 center;
	float radius;
};
// 回転無しボックス
struct AABB_2D {
	Vector2 min;
	Vector2 max;
};
// 回転有りボックス
struct OBB_2D {
	Vector2 center;
	float rotate;
	Vector2 size;
};

class Collision2D
{
public:

	// CircleとAABB_2Dの判定
	static bool CircleAABB(const Circle& circle, const AABB_2D& aabb);

	// OBB_2DとAABB_2Dの判定
	static bool OBBAABB(const OBB_2D& obb, const AABB_2D& aabb);

};

