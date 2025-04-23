#include "Collision3D.h"

#include <algorithm>

bool Collision3D::CollisionChecker(const Sphere& sphere1, const Sphere& sphere2)
{
	float distance = (sphere1.center - sphere2.center).Length();
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	} else {
		return false;
	}
}

bool Collision3D::CollisionChecker(const AABB& aabb, const Sphere& sphere)
{
	//最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};
	//最近接点と球の中心との距離を求める
	float distance = (closestPoint - sphere.center).Length();
	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool Collision3D::CollisionChecker(const AABB& aabb, const Segment& segment)
{
	float tXmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tXmax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tYmin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tYmax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tZmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tZmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = std::min(tXmin, tXmax);
	float tFarX = std::max(tXmin, tXmax);
	float tNearY = std::min(tYmin, tYmax);
	float tFarY = std::max(tYmin, tYmax);
	float tNearZ = std::min(tZmin, tZmax);
	float tFarZ = std::max(tZmin, tZmax);
	//AABBとの衝突点のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax && tmin <= 1.0f && tmax >= 0.0f) {
		return true;
	}
	return false;
}

bool Collision3D::CollisionChecker(const OBB& obb, const Sphere& sphere, std::string& normal)
{
	// OBBのWorld行列を作成
	Matrix4x4 worldInverse = Matrix4x4::Identity();

	for (int32_t i = 0; i < 3; ++i) {
		worldInverse.m[i][0] = obb.rotateMatrix.m[i][0];
		worldInverse.m[i][1] = obb.rotateMatrix.m[i][1];
		worldInverse.m[i][2] = obb.rotateMatrix.m[i][2];
		worldInverse.m[i][3] = 0.0f;
	}

	worldInverse.m[3][0] = obb.center.x;
	worldInverse.m[3][1] = obb.center.y;
	worldInverse.m[3][2] = obb.center.z;
	worldInverse.m[3][3] = 1.0f;

	// SphereをOBBローカル空間に入れる
	Vector3 centerInOBBLocalSpace = Vector3::Transform(sphere.center, Matrix4x4::Inverse(worldInverse));

	// OBBローカルのAABBとOBBローカルのSphereを作成
	AABB aabb_OBBLocal = {
		.min = -obb.size,
		.max = obb.size
	};
	Sphere sphere_OBBLocal = {
		.center = centerInOBBLocalSpace,
		.radius = sphere.radius
	};

	// AABBとSphereで判定を取る
	if (!CollisionChecker(aabb_OBBLocal, sphere_OBBLocal)) {
		normal = "";
		return { false };
	}

	// 最近接点
	Vector3 closestPoint{
		std::clamp(sphere_OBBLocal.center.x, aabb_OBBLocal.min.x, aabb_OBBLocal.max.x),
		std::clamp(sphere_OBBLocal.center.y, aabb_OBBLocal.min.y, aabb_OBBLocal.max.y),
		std::clamp(sphere_OBBLocal.center.z, aabb_OBBLocal.min.z, aabb_OBBLocal.max.z)
	};

	Vector3 normalV = (sphere_OBBLocal.center - closestPoint);
	if (normalV.Length() != 0.0f) { normalV = normalV.Normalize(); }

	// 衝突方向判定
	std::string direction = "";
	float absX = std::abs(normalV.x);
	float absY = std::abs(normalV.y);
	float absZ = std::abs(normalV.z);

	if (absX > absY && absX > absZ) {
		direction = normalV.x > 0 ? "RIGHT" : "LEFT";
	} else if (absY > absZ) {
		direction = normalV.y > 0 ? "UP" : "DOWN";
	} else {
		direction = normalV.z > 0 ? "FRONT" : "BACK";
	}

	normal = direction;
	return { true };
}

bool Collision3D::CollisionChecker(const OBB& obb, const Segment& segment)
{
	// OBBのWorld行列を作成
	Matrix4x4 worldInverse = Matrix4x4::Identity();

	for (int32_t i = 0; i < 3; ++i) {
		worldInverse.m[i][0] = obb.rotateMatrix.m[i][0];
		worldInverse.m[i][1] = obb.rotateMatrix.m[i][1];
		worldInverse.m[i][2] = obb.rotateMatrix.m[i][2];
		worldInverse.m[i][3] = 0.0f;
	}

	worldInverse.m[3][0] = obb.center.x;
	worldInverse.m[3][1] = obb.center.y;
	worldInverse.m[3][2] = obb.center.z;
	worldInverse.m[3][3] = 1.0f;

	// SegmentをOBBローカル空間に入れる
	Vector3 originInOBBLocalSpace = Vector3::Transform(segment.origin, Matrix4x4::Inverse(worldInverse));
	// TransformNormaleにする
	Vector3 diffInOBBLocalSpace = Vector3::Transform(segment.diff, worldInverse);

	// OBBローカルのAABBとOBBローカルのSegmentを作成
	AABB aabb_OBBLocal = {
		.min = -obb.size,
		.max = obb.size
	};
	Segment segment_OBBLocal = {
		.origin = originInOBBLocalSpace,
		.diff = diffInOBBLocalSpace
	};

	// AABBとSegmentで判定を取る
	return CollisionChecker(aabb_OBBLocal, segment_OBBLocal);
}
