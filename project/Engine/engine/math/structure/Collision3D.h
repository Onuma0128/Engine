#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

// 球
struct Sphere {
	Vector3 center;
	float radius;
};
// 線
struct Segment {
	Vector3 origin;
	Vector3 diff;
};
// 回転無しボックス
struct AABB {
	Vector3 min;
	Vector3 max;
};
// 回転有りボックス
struct OBB {
	Vector3 center;
	Matrix4x4 rotateMatrix;
	Vector3 size;
};

class Collision3D
{
public:

	// 球と球の衝突判定
	static bool CollisionChecker(const Sphere& sphere1, const Sphere& sphere2);

	// AABBと球の衝突判定
	static bool CollisionChecker(const AABB& aabb, const Sphere& sphere);

	// AABBとSegmentの衝突判定
	static bool CollisionChecker(const AABB& aabb, const Segment& segment);

	// OBBと球の衝突判定 
	static bool CollisionChecker(const OBB& obb, const Sphere& sphere, std::string& normal);

	// OBBとSegmentの衝突判定
	static bool CollisionChecker(const OBB& obb, const Segment& segment);

};
