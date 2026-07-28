#include "Collision3D.h"

#include <algorithm>

#include "Collider.h"
#include "../../Math/Structure/Quaternion.h"

namespace NumaEngine {

bool Collision3D::SphereSphere(const NumaEngine::Collider* a, const NumaEngine::Collider* b)
{
    Sphere sphere1 = Collision3D::ChangeSphere(a);
	Sphere sphere2 = Collision3D::ChangeSphere(b);

	float distance = (sphere1.center - sphere2.center).Length();
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	} else {
		return false;
	}
}

NumaEngine::Vector3 Collision3D::GetSphereSpherePushVector(const NumaEngine::Collider* a, const NumaEngine::Collider* b) {
    Sphere sA = Collision3D::ChangeSphere(a);
	Sphere sB = Collision3D::ChangeSphere(b);

	NumaEngine::Vector3 dir = sB.center - sA.center;
	float dist = dir.Length();
	float penetration = sA.radius + sB.radius - dist;

	// 重なっていない or ちょうど接触している
    if (penetration <= 0.0f) {
		return NumaEngine::Vector3{ 0, 0, 0 };
	}

	if (dist < 1e-5f) {
		// 完全に重なってる → 適当な方向に押し出す
        dir = NumaEngine::Vector3{ 0, 1, 0 };
		dist = 1.0f;
	}

	// A を B から押し出すベクトル
    NumaEngine::Vector3 pushDir = dir.Normalize();
	return pushDir * penetration;
}


bool Collision3D::AABBSphere(const AABB aabb, const Sphere sphere) {
	//最近接点を求める
	NumaEngine::Vector3 closestPoint{
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

bool Collision3D::AABBSegment(const AABB aabb, const Segment segment)
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

bool Collision3D::OBBSphere(const NumaEngine::Collider* a, const NumaEngine::Collider* b)
{
    OBB obb = Collision3D::ChangeOBB(a);
	Sphere sphere = Collision3D::ChangeSphere(b);

	// OBBのWorld行列を作成
    NumaEngine::Matrix4x4 worldInverse = NumaEngine::Matrix4x4::Identity();

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
    NumaEngine::Vector3 centerInOBBLocalSpace = NumaEngine::Vector3::Transform(sphere.center, NumaEngine::Matrix4x4::Inverse(worldInverse));

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
    if (Collision3D::AABBSphere(aabb_OBBLocal, sphere_OBBLocal)) {
		return true;
	} else {
		return false;
	}
}

NumaEngine::Vector3 Collision3D::GetOBBSpherePushVector(const NumaEngine::Collider* a, const NumaEngine::Collider* b) {
    OBB obb = Collision3D::ChangeOBB(a);
	Sphere sphere = Collision3D::ChangeSphere(b);

	// OBBのワールド→ローカル変換行列
    NumaEngine::Matrix4x4 worldToLocal = NumaEngine::Matrix4x4::Identity();
	for (int i = 0; i < 3; ++i) {
		worldToLocal.m[i][0] = obb.rotateMatrix.m[i][0];
		worldToLocal.m[i][1] = obb.rotateMatrix.m[i][1];
		worldToLocal.m[i][2] = obb.rotateMatrix.m[i][2];
	}
	worldToLocal.m[3][0] = obb.center.x;
	worldToLocal.m[3][1] = obb.center.y;
	worldToLocal.m[3][2] = obb.center.z;
	worldToLocal.m[3][3] = 1.0f;

    NumaEngine::Matrix4x4 localToWorld = NumaEngine::Matrix4x4::Inverse(worldToLocal);
	NumaEngine::Vector3 sphereLocal = NumaEngine::Vector3::Transform(sphere.center, localToWorld);

	AABB aabb = { -obb.size, obb.size };
    NumaEngine::Vector3 closest = {
		std::clamp(sphereLocal.x, aabb.min.x, aabb.max.x),
		std::clamp(sphereLocal.y, aabb.min.y, aabb.max.y),
		std::clamp(sphereLocal.z, aabb.min.z, aabb.max.z)
	};

    NumaEngine::Vector3 pushLocal = sphereLocal - closest;
	float len = pushLocal.Length();

	// ① 通常の押し出し（外部から接触）
	if (len > 1e-5f) {
		float penetration = sphere.radius - len;
		if (penetration > 0.0f) {
            NumaEngine::Vector3 push = pushLocal.Normalize() * penetration;
			return NumaEngine::Vector3::TransformNormal(push, obb.rotateMatrix);
		}
	}

	// ② 完全内部にめり込んでいる場合（中心がAABB内）
	bool isInsideOBB =
		(sphereLocal.x > aabb.min.x && sphereLocal.x < aabb.max.x) &&
		(sphereLocal.y > aabb.min.y && sphereLocal.y < aabb.max.y) &&
		(sphereLocal.z > aabb.min.z && sphereLocal.z < aabb.max.z);

	if (isInsideOBB) {
		// 中心間ベクトルを使った押し出し
        NumaEngine::Vector3 dir = sphere.center - obb.center;
		float dist = dir.Length();
		if (dist < 1e-5f) {
            dir = NumaEngine::Vector3{ 0, 1, 0 }; // ゼロ距離回避
			dist = 1.0f;
		}
		float penetration = sphere.radius;
        NumaEngine::Vector3 push = dir.Normalize() * penetration;
		return push;
	}

	// 押し出し不要
    return NumaEngine::Vector3{};
}

bool Collision3D::SphereSegment(const NumaEngine::Collider* sphereCol,
	const NumaEngine::Collider* segCol)
{
	Sphere   s = ChangeSphere(sphereCol);
	Segment  seg = ChangeSegment(segCol);

	const NumaEngine::Vector3  p0 = seg.origin;				// 始点
	const NumaEngine::Vector3  p1 = seg.origin + seg.diff;	// 終点
	const NumaEngine::Vector3  d = seg.diff;				// 方向ベクトル（長さ＝区間長）
	const float    len2 = NumaEngine::Vector3::Dot(d, d);	// |d|²  ※ゼロ除算対策に使う

	// デジェネレート（長さ０）の場合は点との距離で判定
	if (len2 < 1e-6f) {
		return (p0 - s.center).Length() <= s.radius;
	}

	// 球中心から線分への最近接点を求める
    float t = NumaEngine::Vector3::Dot(s.center - p0, d) / len2;
	t = std::clamp(t, 0.0f, 1.0f);				// 線分内にクランプ
    NumaEngine::Vector3 closest = p0 + d * t;

	// 最近接点と球中心の距離で判定
	return (closest - s.center).Length() <= s.radius;
}

bool Collision3D::SphereSegment(const NumaEngine::Collider* sphereCol,
	const NumaEngine::Collider* segCol,
	RaycastHit* hit)
{
	if (!hit) { return SphereSegment(sphereCol, segCol); }

	Sphere   s = ChangeSphere(sphereCol);
	Segment  seg = ChangeSegment(segCol);
    NumaEngine::Vector3  p0 = seg.origin;
	NumaEngine::Vector3  d = seg.diff;
	float    len2 = NumaEngine::Vector3::Dot(d, d);

	// ① 距離０の線分（＝点）の特別扱い
	if (len2 < 1e-6f) {
		float dist2 = (p0 - s.center).Length();
		if (dist2 > s.radius) { return false; }

		hit->point = p0;
		hit->normal = (p0 - s.center).Normalize();
		hit->t = 0.0f;
		return true;
	}

	// ② 二次方程式で t（線分パラメータ）を解く
    NumaEngine::Vector3 m = p0 - s.center;
	float   b = NumaEngine::Vector3::Dot(m, d);
	float   c = NumaEngine::Vector3::Dot(m, m) - s.radius;
	float   discr = b * b - len2 * c;

	if (discr < 0.0f) { return false; }               // 交差なし

	float sqrtD = std::sqrt(discr);
	float t = (-b - sqrtD) / len2;                    // 入り口側
	if (t < 0.0f || t > 1.0f) {
		t = (-b + sqrtD) / len2;                      // 出口側
		if (t < 0.0f || t > 1.0f) { return false; }
	}

	// ③ ヒット情報を格納
	hit->point = p0 + d * t;
	hit->normal = (hit->point - s.center).Normalize();
	hit->t = t;
	return true;
}

bool Collision3D::OBBSegment(const NumaEngine::Collider* a, const NumaEngine::Collider* b)
{
    OBB obb = Collision3D::ChangeOBB(a);
	Segment segment = ChangeSegment(b);

	// OBBのWorld行列を作成
    NumaEngine::Matrix4x4 worldInverse = NumaEngine::Matrix4x4::Identity();

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
    NumaEngine::Vector3 originInOBBLocalSpace = NumaEngine::Vector3::Transform(segment.origin, NumaEngine::Matrix4x4::Inverse(worldInverse));
	// TransformNormaleにする
    NumaEngine::Vector3 diffInOBBLocalSpace = NumaEngine::Vector3::TransformNormal(segment.diff, NumaEngine::Matrix4x4::Inverse(worldInverse));

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
	return AABBSegment(aabb_OBBLocal, segment_OBBLocal);
}

bool Collision3D::OBBSegment(const NumaEngine::Collider* obbCol, const NumaEngine::Collider* segCol, RaycastHit* hit)
{
	OBB     obb = ChangeOBB(obbCol);
	Segment segW = ChangeSegment(segCol);

    // 1) ローカル変換を既存実装と“同じ作り方”に統一
	NumaEngine::Matrix4x4 worldM = obb.rotateMatrix;
	worldM.m[3][0] = obb.center.x;
	worldM.m[3][1] = obb.center.y;
	worldM.m[3][2] = obb.center.z;
	worldM.m[3][3] = 1.0f;
    NumaEngine::Matrix4x4 invM = NumaEngine::Matrix4x4::Inverse(worldM);

    Segment segL{
		NumaEngine::Vector3::Transform(segW.origin, invM),
		NumaEngine::Vector3::TransformNormal(segW.diff,  invM)
	};

    const NumaEngine::Vector3 min = -obb.size, max = obb.size;

	float tNear = 0.0f, tFar = 1.0f;
	int   axisNear = -1, signNear = 0;
	int   axisFar = -1, signFar = 0;

	auto slab = [&](float o, float d, float smin, float smax, int axis)->bool {
		if (std::abs(d) < 1e-6f) {
			// 平行：レンジ外なら交差なし
			return (o < smin || o > smax);
		}
		float invD = 1.0f / d;
		float t1 = (smin - o) * invD;
		float t2 = (smax - o) * invD;
		int   s1 = (d > 0.0f) ? -1 : 1; // t1 側の法線符号
		int   s2 = -s1;                 // t2 側は反対

		if (t1 > t2) { std::swap(t1, t2); std::swap(s1, s2); }

		if (t1 > tNear) { tNear = t1; axisNear = axis; signNear = s1; }
		if (t2 < tFar) { tFar = t2; axisFar = axis; signFar = s2; }

		// 区間が崩れたら交差なし
		return (tNear > tFar || tFar < 0.0f || tNear > 1.0f);
		};

	if (slab(segL.origin.x, segL.diff.x, min.x, max.x, 0) ||
		slab(segL.origin.y, segL.diff.y, min.y, max.y, 1) ||
		slab(segL.origin.z, segL.diff.z, min.z, max.z, 2)) {
		return false;
	}

	if (!hit) return true;

	// 2) 内側スタート時は tNear < 0 になる。tHit を選び直す
	float   tHit = (tNear >= 0.0f) ? tNear : tFar;
	int     axisHit = (tNear >= 0.0f) ? axisNear : axisFar;
	int     signHit = (tNear >= 0.0f) ? signNear : signFar;
	tHit = std::clamp(tHit, 0.0f, 1.0f);

    NumaEngine::Vector3 pL = segL.origin + segL.diff * tHit;
	NumaEngine::Vector3 nL{ 0,0,0 };
	if (axisHit >= 0) nL[axisHit] = static_cast<float>(signHit);

	// 3) ワールドへ戻す（法線は回転のみ）
    hit->point = NumaEngine::Vector3::Transform(pL, obb.rotateMatrix) + obb.center;
	if (nL.Length() != 0.0f) {
		hit->normal = NumaEngine::Vector3::TransformNormal(nL, obb.rotateMatrix).Normalize();
	}
	hit->t = tHit;
	return true;
}

bool Collision3D::OBBOBB(const NumaEngine::Collider* a, const NumaEngine::Collider* b)
{
	// ① OBB 情報取得
	OBB obb1 = ChangeOBB(a);
	OBB obb2 = ChangeOBB(b);

    const NumaEngine::Vector3 e1 = obb1.size;   // half-extent
	const NumaEngine::Vector3 e2 = obb2.size;

	// ② 各 OBB のローカル軸（列ベクトルを抽出）
    NumaEngine::Vector3 u1[3], u2[3];
	for (int i = 0; i < 3; ++i) {
		u1[i] = NumaEngine::Vector3{ obb1.rotateMatrix.m[i][0], obb1.rotateMatrix.m[i][1], obb1.rotateMatrix.m[i][2] };
		u2[i] = NumaEngine::Vector3{ obb2.rotateMatrix.m[i][0], obb2.rotateMatrix.m[i][1], obb2.rotateMatrix.m[i][2] };
	}

	// ③ 回転行列 R と |R|
	float R[3][3], AbsR[3][3];
	constexpr float kEps = 1e-5f;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
            R[i][j] = NumaEngine::Vector3::Dot(u1[i], u2[j]);
			AbsR[i][j] = std::abs(R[i][j]) + kEps;
		}
	}

	// ④ 中心間ベクトルを u1 基底に変換
    const NumaEngine::Vector3 tWorld = obb2.center - obb1.center;
	const NumaEngine::Vector3 Tvec = {
		NumaEngine::Vector3::Dot(tWorld, u1[0]),
		NumaEngine::Vector3::Dot(tWorld, u1[1]),
		NumaEngine::Vector3::Dot(tWorld, u1[2])
	};

	// ⑤ 分離軸テスト
	// (A の 3 軸)
	for (int i = 0; i < 3; ++i) {
            const float ra = NumaEngine::Vector3::AxisComponent(e1, i);
		const float rb =
			e2.x * AbsR[i][0] + e2.y * AbsR[i][1] + e2.z * AbsR[i][2];
		if (std::abs(NumaEngine::Vector3::AxisComponent(Tvec, i)) > ra + rb) { return false; }
	}

	// (B の 3 軸)
	for (int j = 0; j < 3; ++j) {
		const float ra =
			e1.x * AbsR[0][j] + e1.y * AbsR[1][j] + e1.z * AbsR[2][j];
            const float rb = NumaEngine::Vector3::AxisComponent(e2, j);
			const float proj = std::abs(NumaEngine::Vector3::Dot(tWorld, u2[j]));
		if (proj > ra + rb) { return false; }
	}

	// (交差軸 u1[i] × u2[j] 9 本)
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			const int i1 = (i + 1) % 3, i2 = (i + 2) % 3;
			const int j1 = (j + 1) % 3, j2 = (j + 2) % 3;

            const float ra =
				NumaEngine::Vector3::AxisComponent(e1, i1) * AbsR[i2][j] +
				NumaEngine::Vector3::AxisComponent(e1, i2) * AbsR[i1][j];
			const float rb =
				NumaEngine::Vector3::AxisComponent(e2, j1) * AbsR[i][j2] +
				NumaEngine::Vector3::AxisComponent(e2, j2) * AbsR[i][j1];

            const float Tij = std::abs(
				NumaEngine::Vector3::AxisComponent(Tvec, i2) * R[i1][j] -
				NumaEngine::Vector3::AxisComponent(Tvec, i1) * R[i2][j]);

			if (Tij > ra + rb) { return false; }
		}
	}

	// どの軸でも分離できなければ衝突
	return true;
}

AABB Collision3D::ComputeBroadphaseAABB(const NumaEngine::Collider* c) {
	switch (c->GetMyColliderType()) {
	case NumaEngine::ColliderType::kSphere: {
        Sphere s = ChangeSphere(c);
		NumaEngine::Vector3 r{ s.radius, s.radius, s.radius };
		return { s.center - r, s.center + r };
	}
    case NumaEngine::ColliderType::kSegment: {
        Segment seg = ChangeSegment(c);
		NumaEngine::Vector3 p0 = seg.origin;
		NumaEngine::Vector3 p1 = seg.origin + seg.diff;
		NumaEngine::Vector3 mn{ std::min(p0.x, p1.x), std::min(p0.y, p1.y), std::min(p0.z, p1.z) };
		NumaEngine::Vector3 mx{ std::max(p0.x, p1.x), std::max(p0.y, p1.y), std::max(p0.z, p1.z) };
		return { mn, mx };
	}
    case NumaEngine::ColliderType::kOBB: {
		OBB obb = ChangeOBB(c);
        NumaEngine::Vector3 ex = obb.size;
		NumaEngine::Vector3 ax{
			obb.rotateMatrix.m[0][0], obb.rotateMatrix.m[1][0], obb.rotateMatrix.m[2][0]
		};
		NumaEngine::Vector3 ay{
			obb.rotateMatrix.m[0][1], obb.rotateMatrix.m[1][1], obb.rotateMatrix.m[2][1]
		};
		NumaEngine::Vector3 az{
			obb.rotateMatrix.m[0][2], obb.rotateMatrix.m[1][2], obb.rotateMatrix.m[2][2]
		};
		NumaEngine::Vector3 absAx{ std::abs(ax.x), std::abs(ax.y), std::abs(ax.z) };
		NumaEngine::Vector3 absAy{ std::abs(ay.x), std::abs(ay.y), std::abs(ay.z) };
		NumaEngine::Vector3 absAz{ std::abs(az.x), std::abs(az.y), std::abs(az.z) };
		NumaEngine::Vector3 radius = absAx * ex.x + absAy * ex.y + absAz * ex.z;
		return { obb.center - radius, obb.center + radius };
	}
    default:

		NumaEngine::Vector3 p = c->GetCenterPosition() + c->GetOffsetPosition();
		return { p, p };
	}
}

Sphere Collision3D::ChangeSphere(const NumaEngine::Collider* collider)
{
	return {
		.center = collider->GetCenterPosition() + collider->GetOffsetPosition(),
		.radius = collider->GetRadius()
	};
}

Segment Collision3D::ChangeSegment(const NumaEngine::Collider* collider)
{
	return  {
		.origin = collider->GetOrigin(),
		.diff = collider->GetDiff()
	};
}

AABB Collision3D::ChangeAABB(const NumaEngine::Collider* collider)
{
	return {
		.min = collider->GetCenterPosition() + collider->GetOffsetPosition() - (collider->GetSize() * 0.5f),
		.max = collider->GetCenterPosition() + collider->GetOffsetPosition() + (collider->GetSize() * 0.5f)
	};
}

OBB Collision3D::ChangeOBB(const NumaEngine::Collider* collider)
{
    auto rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(collider->GetRotate());
	return {
		.center = collider->GetCenterPosition() + collider->GetOffsetPosition().Transform(rotateMatrix),
		.rotateMatrix = rotateMatrix,
		.size = collider->GetSize()
	};
}

} // namespace NumaEngine
