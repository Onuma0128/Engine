#pragma once

#include <memory>
#include <vector>

#include "Line3d.h"
#include "Vector3.h"

/// <summary>
/// 探索アルゴリズムのスプラインを管理するクラス
/// </summary>
class SplineMover
{
public:

	/// <summary>
	/// スプラインをリセットする
	/// </summary>
	void Reset();

	// スプライン座標の更新

	/// <summary>
	/// スプラインを更新
	/// </summary>
	/// <param name="speed"></スプラインを進むスピードを設定する>
	/// <param name="lookAt_t"></0 ～ 1のtimeを入れる>
	void Update(const float speed, float lookAt_t);

	/// <summary>
	/// スプラインの各セグメントの長さを計算する
	/// </summary>
	void ComputeArcLengths();

	/// <summary>
	/// 現在のスプライン上の速度を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() { return velocity_; }

	/// <summary>
	/// 現在のスプライン上の座標を取得する
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() { return position_; }

	/// <summary>
	/// 現在のスプライン上のy回転のクォータニオンを取得する
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() { return yRotation_; }

	/// <summary>
	/// スプラインのデバッグ描画を有効/無効にする
	/// </summary>
	/// <param name="flag"></param>
	void DebugSpline(bool flag) { 
		if (splines_) {
			splines_->GetMaterial().enableDraw = flag;
			splines_->Update();
		}
	}

	/// <summary>
	/// スプラインの座標を逆順にする
	/// </summary>
	void ReverseSplinePositions() {
		std::reverse(splinePositions_.begin(),splinePositions_.end());
	}

	/// <summary>
	/// スプラインの座標を追加する
	/// </summary>
	/// <param name="position"></param>
	void PushSplinePositions(const Vector3& position) {
		splinePositions_.push_back(position);
	}

	/// <summary>
	/// スプラインの座標をクリアする
	/// </summary>
	void ClearSplinePositions() { splinePositions_.clear(); currentDistance_ = 0.0f; }

	/// <summary>
	/// スプラインの座標を削除する
	/// </summary>
	void BackSplinePositions() { splinePositions_.pop_back(); }

private:

	// 速度 * 時間を入れる

	/// <summary>
	/// スプラインの各セグメントの長さを取得する
	/// </summary>
	/// <param name="targetDistance"></param>
	/// <returns></returns>
	float GetArcLengthParam(float targetDistance);

	/// <summary>
	/// スプラインの各セグメントの長さを取得する
	/// </summary>
	/// <param name="linePositions"></ラインの配列を取得する>
	/// <param name="start"></座標のスタート>
	/// <param name="end"></座標の終了>
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

