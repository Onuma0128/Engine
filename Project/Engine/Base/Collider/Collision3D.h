#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

// 球
struct Sphere {
	NumaEngine::Vector3 center;
	float radius;
};
// 線
struct Segment {
	NumaEngine::Vector3 origin;
	NumaEngine::Vector3 diff;
};
// レイ
struct RaycastHit {
	NumaEngine::Vector3 point;
	NumaEngine::Vector3 normal;
	float   t;
};
// 回転無しボックス
struct AABB {
	NumaEngine::Vector3 min;
	NumaEngine::Vector3 max;
};
// 回転有りボックス
struct OBB {
	NumaEngine::Vector3 center;
    NumaEngine::Matrix4x4 rotateMatrix;
	NumaEngine::Vector3 size;
};

/// <summary>
/// 3D衝突判定クラス
/// </summary>
namespace NumaEngine {

class Collider;


class Collision3D
{
public:

	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	/// <param name="a"></球A>
	/// <param name="b"></球B>
	/// <returns></returns>
    static bool SphereSphere(const NumaEngine::Collider* a, const NumaEngine::Collider* b);
	/// <summary>
	/// 球と球の押し出し処理
	/// </summary>
	/// <param name="a"></球A>
	/// <param name="b"></球B>
	/// <returns></接触した座標>
    static NumaEngine::Vector3 GetSphereSpherePushVector(const NumaEngine::Collider* a, const NumaEngine::Collider* b);

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
    static bool OBBSphere(const NumaEngine::Collider* a, const NumaEngine::Collider* b);
	
	/// <summary>
	/// OBBと球の押し出し処理
	/// </summary>
	/// <param name="a"></球>
	/// <param name="b"></回転ボックス>
	/// <returns></接触した座標>
    static NumaEngine::Vector3 GetOBBSpherePushVector(const NumaEngine::Collider* a, const NumaEngine::Collider* b);

	/// <summary>
	/// 球とSegmentの衝突判定
	/// </summary>
	/// <param name="sphereCol"></球>
	/// <param name="segCol"></線>
	/// <returns></returns>
    static bool SphereSegment(const NumaEngine::Collider* sphereCol,const NumaEngine::Collider* segCol);
	static bool SphereSegment(const NumaEngine::Collider* sphereCol, const NumaEngine::Collider* segCol, RaycastHit* hit);

	/// <summary>
	/// OBBとSegmentの衝突判定
	/// </summary>
	/// <param name="a"></回転ボックス>
	/// <param name="b"></線>
	/// <returns></returns>
    static bool OBBSegment(const NumaEngine::Collider* a, const NumaEngine::Collider* b);
	static bool OBBSegment(const NumaEngine::Collider* obbCol, const NumaEngine::Collider* segCol, RaycastHit* hit);

	/// <summary>
	/// OBBとOBBの衝突判定
	/// </summary>
	/// <param name="a"></回転ボックスA>
	/// <param name="b"></回転ボックスB>
	/// <returns></returns>
    static bool OBBOBB(const NumaEngine::Collider* a, const NumaEngine::Collider* b);

	/// <summary>
	/// ブロードフェーズAABBを計算する
	/// </summary>
	/// <param name="collider"></param>
	/// <returns></returns>
    static AABB ComputeBroadphaseAABB(const NumaEngine::Collider* collider);

	// コライダーから各形状に変換する

static Sphere ChangeSphere(const NumaEngine::Collider* collider);
static Segment ChangeSegment(const NumaEngine::Collider* collider);
static AABB ChangeAABB(const NumaEngine::Collider* collider);
static OBB ChangeOBB(const NumaEngine::Collider* collider);
};

} // namespace NumaEngine



