#include "AlgorithmTest.h"

#include "imgui.h"
#include "DeltaTimer.h"

#include "searchAlgorithm/collision/MapCollision.h"

void AlgorithmTest::Init()
{
    NumaEngine::Object3d::Initialize("Box.obj");
	NumaEngine::Object3d::SetSceneRenderer();

	Reset({});
}

void AlgorithmTest::Update(const NumaEngine::Vector3& goal)
{
	ImGui::Begin("A_star");
	ImGui::DragFloat("speed", &speed_, 0.01f);
	ImGui::DragFloat("lerpSpeed", &lerpSpeed_, 0.01f);
	ImGui::DragFloat("SearchTime", &searchDecisionTime_, 0.01f);
	ImGui::Checkbox("drawSpline", &drawSpline_);
	if (ImGui::Button("start")) {
		Reset(goal);
	}
	ImGui::End();

    // 自動で探索をする
	isSearchTime_ += NumaEngine::DeltaTimer::GetDeltaTime();
	if (isSearchTime_ >= searchDecisionTime_) {
		isSearchTime_ = 0.0f;
		Reset(goal);
	}

	// 時間や座標、回転の更新
	pathFinder_.Update(speed_);
	pathFinder_.DebugSpline(drawSpline_);

    // 移動をする
	NumaEngine::Vector3 position = pathFinder_.GetPosition();
	transform_.translation_ = position;
	// 回転をする
	NumaEngine::Quaternion yRotation = pathFinder_.GetRotation();
	transform_.rotation_ = NumaEngine::Quaternion::Slerp(transform_.rotation_, yRotation, lerpSpeed_);

	NumaEngine::Object3d::Update();
}

void AlgorithmTest::Reset(const NumaEngine::Vector3& goal)
{
    pathFinder_.Search(transform_.translation_, goal);
}
