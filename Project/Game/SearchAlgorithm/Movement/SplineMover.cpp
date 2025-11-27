#include "SplineMover.h"

#include "DeltaTimer.h"

void SplineMover::Reset()
{
	splinePositions_.clear();
	arcLengths_.clear();
}

void SplineMover::Update(const float speed, float lookAt_t)
{

#ifdef ENABLE_EDITOR
	if (splines_ != nullptr) {
		splines_->GetMaterial().enableDraw = false;
		splines_->Update();
	}
#endif // ENABLE_EDITOR

	// 速度に応じて距離を進める
	currentDistance_ += speed * DeltaTimer::GetDeltaTime();
	float t = GetArcLengthParam(currentDistance_);

	if (splinePositions_.empty()) { return; }
	position_ = Vector3::CatmullRomPosition(splinePositions_, t);
	lookAtPosition_ = Vector3::CatmullRomPosition(splinePositions_, t + lookAt_t);

	if (t < 1.0f) {
		// ベクトルの方向に回転
		velocity_ = { lookAtPosition_ - position_ };
		Vector3 targetDirection = { -velocity_.x, 0.0f, velocity_.z };
		Vector3 currentDirection = Vector3::ExprUnitZ;
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection, targetDirection);
		yRotation_ = Quaternion::FormRotationMatrix(rotationMatrix);
	}


}

void SplineMover::ComputeArcLengths()
{
	// スプラインサイズが4より小さければ即return
	if (splinePositions_.size() <= 1) {
		splinePositions_.clear();
		return;
	}
	std::vector<Vector3> linePositions;

	arcLengths_.clear();
	float totalLength = 0.0f;
	arcLengths_.push_back(0.0f); // 始点

	Vector3 prevPos = Vector3::CatmullRomPosition(splinePositions_, 0.0f);
	const int kNumSamples = 100; // サンプリング数

	for (int i = 1; i <= kNumSamples; i++) {
		float t = static_cast<float>(i) / kNumSamples;
		Vector3 currentPos = Vector3::CatmullRomPosition(splinePositions_, t);
		totalLength += (currentPos - prevPos).Length(); // 距離を積算
		arcLengths_.push_back(totalLength);
#ifdef ENABLE_EDITOR
		SetLinePosition(linePositions, prevPos, currentPos);
#endif // ENABLE_EDITOR
		prevPos = currentPos;
	}

#ifdef ENABLE_EDITOR
	// ラインの初期化
	if (splines_ == nullptr) {
		splines_ = std::make_unique<Line3d>();
		splines_->Initialize(linePositions);
		splines_->SetColor(Vector3::ExprUnitZ);
	} else {
		splines_->SetPositions(linePositions);
		splines_->Update();
	}
#endif // ENABLE_EDITOR
}

float SplineMover::GetArcLengthParam(float targetDistance)
{
	for (size_t i = 1; i < arcLengths_.size(); i++) {
		if (arcLengths_[i] >= targetDistance) {
			float segmentLength = arcLengths_[i] - arcLengths_[i - 1];
			float segmentT = (static_cast<float>(i) - 1) / (arcLengths_.size() - 1);
			float nextSegmentT = static_cast<float>(i) / (arcLengths_.size() - 1);
			float lerpFactor = (targetDistance - arcLengths_[i - 1]) / segmentLength;
			return segmentT + (nextSegmentT - segmentT) * lerpFactor;
		}
	}
	return 1.0f; // 終点
}

void SplineMover::SetLinePosition(std::vector<Vector3>& linePositions, const Vector3& start, const Vector3& end)
{
	const float kY = 0.25f;

	linePositions.push_back(Vector3{ start.x,kY,start.z });
	linePositions.push_back(Vector3{ end.x,kY,end.z });
}
