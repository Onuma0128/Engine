#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Vector3.h"
#include "Quaternion.h"

class Line3d;

/// <summary>
/// コライダーの種類
/// </summary>
enum class ColliderType 
{
	Sphere,		// 球体
	Segment,	// 線
	OBB			// 回転直方体
};

/// <summary>
/// コライダー基底クラス
/// </summary>
class Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collider();

	/// <summary>
	/// コライダーを追加する
	/// </summary>
	virtual void AddCollider();

	/// <summary>
	/// コライダーを削除する
	/// </summary>
	virtual void RemoveCollider();

	/// <summary>
	/// コライダーを描画する
	/// </summary>
	virtual void DrawCollider();

	/// <summary>
	/// コライダーの更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ラインコライダーの更新
	/// </summary>
	virtual void LineUpdate();

	/// <summary>
	/// 当たり判定の処理
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	// 当たり判定が発生したときに呼ばれる
	virtual void OnCollisionEnter(Collider* other) { other; };
	// 当たり判定が継続しているときに呼ばれる
	virtual void OnCollisionStay(Collider* other) { other; };
	// 当たり判定が終了したときに呼ばれる
	virtual void OnCollisionExit(Collider* other) { other; };

	/// <summary>
	/// アクセッサ
	/// </summary>
	
	// 自分のコライダーのタイプ取得
	virtual const ColliderType& GetMyColliderType()const { return myType_; }
	// 対象のコライダーのタイプ取得
	virtual const ColliderType& GetTargetColliderType()const { return targetType_; }
	// コライダーが有効か取得
	virtual const bool& GetActive()const { return isActive_; }
	// コライダーのサイズ、回転、中心座標、オフセット座標取得
	virtual const Vector3& GetSize() const { return size_; }
	virtual const Quaternion& GetRotate() const { return rotate_; }
	virtual const Vector3& GetCenterPosition() const { return centerPosition_; }
	virtual const Vector3& GetOffsetPosition() const { return offsetPosition_; }

	// コライダーの名前取得
	virtual const std::string& GetColliderName()const { return colliderName_; }
	// 対象コライダーの名前取得
	virtual const std::vector<std::string>& GetTargetColliderName()const { return targetColliderName_; }
	// コライダーの半径取得
	virtual const float& GetRadius()const { return radius_; }
	// セグメントの始点、方向ベクトル取得
	virtual const Vector3& GetOrigin()const { return origin_; }
	// セグメントの方向ベクトル取得
	virtual const Vector3& GetDiff()const { return diff_; }

	// コライダーのカラー設定
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

	/// <summary>
	/// ラインの位置情報を生成する
	/// </summary>
	/// <returns></returns>
	std::vector<Vector3> CreateLinePositions();

};
