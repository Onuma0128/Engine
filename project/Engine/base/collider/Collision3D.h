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
// レイ
struct RaycastHit {
	Vector3 point;
	Vector3 normal;
	float   t;
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

class Collider;

/// <summary>
/// 3D衝突判定クラス
/// </summary>
class Collision3D
{
public:

	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="a"></球A>
	/// <param name="b"></球B>
	/// <returns></returns>
	static bool SphereSphere(const Collider* a, const Collider* b);
	/// <summary>
	/// 球と球の押し出し処理
	/// </summary>
	/// <param name="a"></球A>
	/// <param name="b"></球B>
	/// <returns></接触した座標>
	static Vector3 GetSphereSpherePushVector(const Collider* a, const Collider* b);

	/// <summary>
	/// AABBと球の衝突判定
	/// </summary>
	/// <param name="aabb"></回転無しボックス>
	/// <param name="sphere"></球>
	/// <returns></returns>
	static bool AABBSphere(const AABB aabb, const Sphere sphere);

	/// <summary>
	/// AABBとSegmentの衝突判定
	/// </summary>
	/// <param name="aabb"></回転無しボックス>
	/// <param name="segment"></線>
	/// <returns></returns>
	static bool AABBSegment(const AABB aabb, const Segment segment);

	/// <summary>
	/// OBBと球の衝突判定
	/// </summary>
	/// <param name="a"></球>
	/// <param name="b"></回転ボックス>
	/// <returns></returns>
	static bool OBBSphere(const Collider* a, const Collider* b);
	
	/// <summary>
	/// OBBと球の押し出し処理
	/// </summary>
	/// <param name="a"></球>
	/// <param name="b"></回転ボックス>
	/// <returns></接触した座標>
	static Vector3 GetOBBSpherePushVector(const Collider* a, const Collider* b);

	/// <summary>
	/// 球とSegmentの衝突判定
	/// </summary>
	/// <param name="sphereCol"></球>
	/// <param name="segCol"></線>
	/// <returns></returns>
	static bool SphereSegment(const Collider* sphereCol,const Collider* segCol);
	static bool SphereSegment(const Collider* sphereCol, const Collider* segCol, RaycastHit* hit);

	/// <summary>
	/// OBBとSegmentの衝突判定
	/// </summary>
	/// <param name="a"></回転ボックス>
	/// <param name="b"></線>
	/// <returns></returns>
	static bool OBBSegment(const Collider* a, const Collider* b);
	static bool OBBSegment(const Collider* obbCol, const Collider* segCol, RaycastHit* hit);

	/// <summary>
	/// OBBとOBBの衝突判定
	/// </summary>
	/// <param name="a"></回転ボックスA>
	/// <param name="b"></回転ボックスB>
	/// <returns></returns>
	static bool OBBOBB(const Collider* a, const Collider* b);

	/// <summary>
	/// ブロードフェーズAABBを計算する
	/// </summary>
	/// <param name="collider"></param>
	/// <returns></returns>
	static AABB ComputeBroadphaseAABB(const Collider* collider);

	// コライダーから各形状に変換する
	static Sphere ChangeSphere(const Collider* collider);
	static Segment ChangeSegment(const Collider* collider);
	static AABB ChangeAABB(const Collider* collider);
	static OBB ChangeOBB(const Collider* collider);
};
