#include "CompanionAdjustItem.h"

#include "imgui.h"

void CompanionAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */
	mainJson_.Init("CompanionMain");
	if (!mainJson_.Load()) {
		mainJson_.Set("speed", 0.0f);
		mainJson_.Set("hitPushSpeed", 0.0f);
		mainJson_.Set("searchUpdateTime", 0.0f);
		mainJson_.Set("searchCancelDistance", 0.0f);
		mainJson_.Set("colliderSize", 0.0f);
		mainJson_.Set("colliderOffset", Vector3{});
		mainJson_.Set("dashSpeed", 0.0f);
	} else {
		mainData_.speed = mainJson_.Get("speed", mainData_.speed);
		mainData_.hitPushSpeed = mainJson_.Get("hitPushSpeed", mainData_.hitPushSpeed);
		mainData_.colliderSize = mainJson_.Get("colliderSize", mainData_.colliderSize);
		mainData_.colliderOffset = mainJson_.Get("colliderOffset", mainData_.colliderOffset);
		mainData_.searchUpdateTime = mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
		mainData_.searchCancelDistance = mainJson_.Get("searchCancelDistance", mainData_.searchCancelDistance);
		mainData_.dashSpeed = mainJson_.Get("dashSpeed", mainData_.dashSpeed);
	}
	mainData_.debugSpline = false;
}

void CompanionAdjustItem::Editor()
{
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("CompanionParameters")) {
		ImGui::Separator();
		ImGui::PushItemWidth(150);

		ImGui::DragFloat("speed", &mainData_.speed, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("hitPushSpeed", &mainData_.hitPushSpeed, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("dashSpeed", &mainData_.dashSpeed, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f, 0.0f, 100.0f);

		ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("searchCancelDistance", &mainData_.searchCancelDistance, 0.01f, 0.0f, 100.0f);
		ImGui::Checkbox("debugSpline", &mainData_.debugSpline);
		if (ImGui::Button("Save")) {
			mainJson_.Set("speed", mainData_.speed);
			mainJson_.Set("hitPushSpeed", mainData_.hitPushSpeed);
			mainJson_.Set("dashSpeed", mainData_.dashSpeed);
			mainJson_.Set("colliderSize", mainData_.colliderSize);
			mainJson_.Set("colliderOffset", mainData_.colliderOffset);
			mainJson_.Set("searchUpdateTime", mainData_.searchUpdateTime);
			mainJson_.Set("searchCancelDistance", mainData_.searchCancelDistance);
			mainJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}