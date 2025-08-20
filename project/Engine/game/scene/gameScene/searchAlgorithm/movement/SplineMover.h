#pragma once

#include <memory>
#include <vector>

#include "Line3d.h"
#include "Vector3.h"

class SplineMover
{
public:

	void Reset();

	// スプライン座標の更新
	void Update(const float speed, float lookAt_t);

	// 各スプライン上の距離を格納する
	void ComputeArcLengths();

	// 今の座標と回転を取得
	const Vector3& GetVelocity() { return velocity_; }
	const Vector3& GetPosition() { return position_; }
	const Quaternion& GetRotation() { return yRotation_; }
	void DebugSpline(bool flag) { 
		if (splines_) {
			splines_->GetMaterial().enableDraw = flag;
			splines_->Update();
		}
	}

	// スプラインの座標を取得
	void ReverseSplinePositions() {
		std::reverse(splinePositions_.begin(),splinePositions_.end());
	}
	void PushSplinePositions(const Vector3& position) {
		splinePositions_.push_back(position);
	}
	void ClearSplinePositions() { splinePositions_.clear(); currentDistance_ = 0.0f; }
	void BackSplinePositions() { splinePositions_.pop_back(); }

private:

	// 速度 * 時間を入れる
	float GetArcLengthParam(float targetDistance);

	void SetLinePosition(std::vector<Vector3>& linePositions, const Vector3& start, const Vector3& end);

private:

	// アルゴリズムで出た座標を格納
	std::vector<Vector3> splinePositions_;
	std::unique_ptr<Line3d> splines_;
	// 累積距離
	std::vector<float> arcLengths_;

	// 速度 * 時間
	float currentDistance_;

	// 今の座標と次の座標
	Vector3 position_;
	Vector3 lookAtPosition_;
	Vector3 velocity_;
	// 今の座標から見た回転
	Quaternion yRotation_;

};

