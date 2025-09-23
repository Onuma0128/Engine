#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Vector3.h"
#include "Quaternion.h"

class Line3d;

enum class ColliderType 
{
	Sphere,		// 球体
	Segment,	// 線
	OBB			// 回転直方体
};

class Collider
{
public:

	virtual ~Collider();
	virtual void AddCollider();
	virtual void RemoveCollider();
	virtual void DrawCollider();
	virtual void Update();
	virtual void LineUpdate();

	virtual void OnCollisionEnter(Collider* other) {};
	virtual void OnCollisionStay(Collider* other) {};
	virtual void OnCollisionExit(Collider* other) {};

	virtual const ColliderType& GetMyColliderType()const { return myType_; }
	virtual const ColliderType& GetTargetColliderType()const { return targetType_; }
	virtual const bool& GetActive()const { return isActive_; }

	virtual const Vector3& GetSize() const { return size_; }
	virtual const Quaternion& GetRotate() const { return rotate_; }
	virtual const Vector3& GetCenterPosition() const { return centerPosition_; }
	virtual const Vector3& GetOffsetPosition() const { return offsetPosition_; }

	virtual const std::string& GetColliderName()const { return colliderName_; }
	virtual const std::vector<std::string>& GetTargetColliderName()const { return targetColliderName_; }
	virtual const float& GetRadius()const { return radius_; }
	virtual const Vector3& GetOrigin()const { return origin_; }
	virtual const Vector3& GetDiff()const { return diff_; }

	void SetColor(const Vector3& color);
	void SetDebugDrawEnable(bool flag);

protected:

	// コライダーの名前
	std::string colliderName_ = "name";
	std::vector<std::string> targetColliderName_;

	// コライダーのType
	ColliderType myType_ = ColliderType::Sphere;
	ColliderType targetType_ = ColliderType::Sphere;

	// コライダーをON/OFF
	bool isActive_ = true;
	bool debugDrawEnable_ = false;

	// 中心座標
	Vector3 centerPosition_;
	// 中心座標からどのくらいズレているか
	Vector3 offsetPosition_;
	
	// OBB用(サイズと回転)
	Vector3 size_;
	Quaternion rotate_;

	// Sphere用(半径)
	float radius_;

	// Segment用
	Vector3 origin_;
	Vector3 diff_;

private:

	std::shared_ptr<Line3d> line_ = nullptr;
	std::vector<Vector3> linePositions_;

	std::vector<Vector3> CreateLinePositions();

};
