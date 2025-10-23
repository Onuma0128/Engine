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

/// <summary>
/// 2D衝突判定クラス
/// </summary>
class Collision2D
{
public:

	/// <summary>
	/// CircleとAABB_2Dの判定
	/// </summary>
	/// <param name="circle"></円>
	/// <param name="aabb"></矩形>
	/// <returns></returns>
	static bool CircleAABB(const Circle& circle, const AABB_2D& aabb);

	/// <summary>
	/// OBB_2DとAABB_2Dの判定
	/// </summary>
	/// <param name="obb"></回転矩形>
	/// <param name="aabb"></矩形>
	/// <returns></returns>
	static bool OBBAABB(const OBB_2D& obb, const AABB_2D& aabb);

};

