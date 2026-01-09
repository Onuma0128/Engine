#include "GameUiAdjustItem.h"

#include "imgui.h"

void GameUiAdjustItem::LoadItems()
{
	json_.Init("GameSceneUi");
	if (!json_.Load()) {

		json_.Set("killNumberUiInterval", 0.0f);
		json_.Set("killNumberUiSize", Vector2{});
		json_.Set("killNumberUiPos", Vector2{});

		json_.Set("maxKillCount", 0);
		json_.Set("maxKillNumberUiInterval", 0.0f);
		json_.Set("maxKillNumberUiSize", Vector2{});
		json_.Set("maxKillNumberUiPos", Vector2{});

		json_.Set("startInTime", 0.0f);
		json_.Set("startOutTime", 0.0f);
		json_.Set("activeTime", 0.0f);
		json_.Set("recoverInTime", 0.0f);
		json_.Set("recoverOutTime", 0.0f);

	} else {

		data_.killNumberUiInterval = json_.Get("killNumberUiInterval", data_.killNumberUiInterval);
		data_.killNumberUiSize = json_.Get("killNumberUiSize", data_.killNumberUiSize);
		data_.killNumberUiPos = json_.Get("killNumberUiPos", data_.killNumberUiPos);

		data_.maxKillCount = json_.Get("maxKillCount", data_.maxKillCount);
		data_.maxKillNumberUiInterval = json_.Get("maxKillNumberUiInterval", data_.maxKillNumberUiInterval);
		data_.maxKillNumberUiSize = json_.Get("maxKillNumberUiSize", data_.maxKillNumberUiSize);
		data_.maxKillNumberUiPos = json_.Get("maxKillNumberUiPos", data_.maxKillNumberUiPos);

		data_.startInTime = json_.Get("startInTime", data_.startInTime);
		data_.startOutTime = json_.Get("startOutTime", data_.startOutTime);
		data_.activeTime = json_.Get("activeTime", data_.activeTime);
		data_.recoverInTime = json_.Get("recoverInTime", data_.recoverInTime);
		data_.recoverOutTime = json_.Get("recoverOutTime", data_.recoverOutTime);
	}
}

void GameUiAdjustItem::Editor()
{
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("GameSceneUi")) {
		ImGui::Separator();

		ImGui::PushItemWidth(150);

		ImGui::DragFloat("killNumberUiInterval", &data_.killNumberUiInterval);
		ImGui::DragFloat2("killNumberUiSize", &data_.killNumberUiSize.x);
		ImGui::DragFloat2("killNumberUiPos", &data_.killNumberUiPos.x);

		ImGui::DragInt("maxKillCount", &data_.maxKillCount);
		ImGui::DragFloat("maxKillNumberUiInterval", &data_.maxKillNumberUiInterval);
		ImGui::DragFloat2("maxKillNumberUiSize", &data_.maxKillNumberUiSize.x);
		ImGui::DragFloat2("maxKillNumberUiPos", &data_.maxKillNumberUiPos.x);

		ImGui::DragFloat("startInTime", &data_.startInTime);
		ImGui::DragFloat("startOutTime", &data_.startOutTime);
		ImGui::DragFloat("activeTime", &data_.activeTime);
		ImGui::DragFloat("recoverInTime", &data_.recoverInTime);
		ImGui::DragFloat("recoverOutTime", &data_.recoverOutTime);

		if (ImGui::Button("Save")) {
			Save();
		}

		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}

void GameUiAdjustItem::Save()
{
	json_.Set("killNumberUiInterval", data_.killNumberUiInterval);
	json_.Set("killNumberUiSize", data_.killNumberUiSize);
	json_.Set("killNumberUiPos", data_.killNumberUiPos);

	json_.Set("maxKillCount", data_.maxKillCount);
	json_.Set("maxKillNumberUiInterval", data_.maxKillNumberUiInterval);
	json_.Set("maxKillNumberUiSize", data_.maxKillNumberUiSize);
	json_.Set("maxKillNumberUiPos", data_.maxKillNumberUiPos);

	json_.Set("startInTime", data_.startInTime);
	json_.Set("startOutTime", data_.startOutTime);
	json_.Set("activeTime", data_.activeTime);
	json_.Set("recoverInTime", data_.recoverInTime);
	json_.Set("recoverOutTime", data_.recoverOutTime);

	json_.Save();
}