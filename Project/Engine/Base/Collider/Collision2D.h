#pragma once

#include "Vector2.h"
#include "Vector3.h"

// Ensure Windows min/max macros don't break member names like 'min' and 'max'
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace NumaEngine {

// 円
struct Circle {
	NumaEngine::Vector2 center;
	float radius;
};
// 回転無しボックス
struct AABB_2D {
	NumaEngine::Vector2 min;
	NumaEngine::Vector2 max;
};
// 回転有りボックス
struct OBB_2D {
	NumaEngine::Vector2 center;
	float rotate;
	NumaEngine::Vector2 size;
	NumaEngine::Vector3 worldPosition;
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

} // namespace NumaEngine




