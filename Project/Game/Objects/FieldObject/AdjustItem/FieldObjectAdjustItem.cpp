#include "FieldObjectAdjustItem.h"

#include "imgui.h"

void FieldObjectAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */
	mainJson_.Init("FieldObjectMain");
	if (!mainJson_.Load()) {
		mainJson_.Set("shakePower", 0.0f);
		mainJson_.Set("shakeOffset", Vector3{});
		mainJson_.Set("cameraShakePower", 0.0f);
		mainJson_.Set("breakTimer", 0.0f);
		mainJson_.Set("breakEmitTimer", 0.0f);
	} else {
		mainData_.shakePower =			mainJson_.Get("shakePower", mainData_.shakePower);
		mainData_.shakeOffset =			mainJson_.Get("shakeOffset", mainData_.shakeOffset);
		mainData_.cameraShakePower =	mainJson_.Get("cameraShakePower", mainData_.cameraShakePower);
		mainData_.breakTimer =			mainJson_.Get("breakTimer", mainData_.breakTimer);
		mainData_.breakEmitTimer =		mainJson_.Get("breakEmitTimer", mainData_.breakEmitTimer);
	}
}

void FieldObjectAdjustItem::Editor()
{
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("FieldObjectParameters")) {
		ImGui::Separator();
		ImGui::PushItemWidth(150);

		ImGui::DragFloat("shakePower", &mainData_.shakePower, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat3("shakeOffset", &mainData_.shakeOffset.x, 0.01f, -100.0f, 100.0f);
		ImGui::DragFloat("cameraShakePower", &mainData_.cameraShakePower, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("breakTimer", &mainData_.breakTimer, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("breakEmitTimer", &mainData_.breakEmitTimer, 0.01f, 0.0f, 100.0f);
		if (ImGui::Button("Save")) {
			mainJson_.Set("shakePower", mainData_.shakePower);
			mainJson_.Set("shakeOffset", mainData_.shakeOffset);
			mainJson_.Set("cameraShakePower", mainData_.cameraShakePower);
			mainJson_.Set("breakTimer", mainData_.breakTimer);
			mainJson_.Set("breakEmitTimer", mainData_.breakEmitTimer);
			mainJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}