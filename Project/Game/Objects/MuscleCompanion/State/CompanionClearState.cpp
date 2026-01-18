#include "CompanionClearState.h"

#include <numbers>

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

CompanionClearState::CompanionClearState(MuscleCompanion* companion) : CompanionBaseState(companion) {}

void CompanionClearState::Init()
{
	companion_->ForcePlayByName("WinPose", 0.1f);
	companion_->GetTimeStop() = true;
}

void CompanionClearState::Finalize()
{
}

void CompanionClearState::Update()
{
	// 回転をしてカメラ方向に向ける
	Quaternion preRotate = companion_->GetTransform().rotation_;
	Quaternion postRotate = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, std::numbers::pi_v<float>);

	companion_->SetTransformRotation(Quaternion::Lerp(preRotate, postRotate, 0.2f));
}

void CompanionClearState::Draw()
{
}

