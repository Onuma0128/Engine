#include "SelectAdjustItem.h"

#include "imgui.h"

void SelectAdjustItem::LoadItems()
{
	json_.Init("SelectSystem");
	if (!json_.Load()) {

		json_.Set("killNumberUiInterval", 0.0f);
		json_.Set("killNumberUiSize", Vector2{});
		json_.Set("killNumberUiPos", Vector2{});

		json_.Set("hitRateNumberUiInterval", 0.0f);
		json_.Set("hitRateNumberUiSize", Vector2{});
		json_.Set("hitRateNumberUiPos", Vector2{});

		json_.Set("selectUiInterval", 0.0f);
		json_.Set("clearCountUiTimer", 0.0f);

	} else {

		data_.killNumberUiInterval = json_.Get("killNumberUiInterval", data_.killNumberUiInterval);
		data_.killNumberUiSize = json_.Get("killNumberUiSize", data_.killNumberUiSize);
		data_.killNumberUiPos = json_.Get("killNumberUiPos", data_.killNumberUiPos);

		data_.hitRateNumberUiInterval = json_.Get("hitRateNumberUiInterval", data_.hitRateNumberUiInterval);
		data_.hitRateNumberUiSize = json_.Get("hitRateNumberUiSize", data_.hitRateNumberUiSize);
		data_.hitRateNumberUiPos = json_.Get("hitRateNumberUiPos", data_.hitRateNumberUiPos);

		data_.selectUiInterval = json_.Get("selectUiInterval", data_.selectUiInterval);
		data_.clearCountUiTimer_ = json_.Get("clearCountUiTimer", data_.clearCountUiTimer_);
	}
}

void SelectAdjustItem::Editor()
{
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("SelectSystem")) {
		ImGui::Separator();

		ImGui::PushItemWidth(150);

		ImGui::DragFloat("killNumberUiInterval", &data_.killNumberUiInterval);
		ImGui::DragFloat2("killNumberUiSize", &data_.killNumberUiSize.x);
		ImGui::DragFloat2("killNumberUiPos", &data_.killNumberUiPos.x);

		ImGui::DragFloat("hitRateNumberUiInterval", &data_.hitRateNumberUiInterval);
		ImGui::DragFloat2("hitRateNumberUiSize", &data_.hitRateNumberUiSize.x);
		ImGui::DragFloat2("hitRateNumberUiPos", &data_.hitRateNumberUiPos.x);

		ImGui::DragFloat("selectUiInterval", &data_.selectUiInterval, 0.01f);
		ImGui::DragFloat("clearCountUiTimer", &data_.clearCountUiTimer_, 0.01f);

		if (ImGui::Button("Save")) {
			Save();
		}

		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}

void SelectAdjustItem::Save()
{
	json_.Set("killNumberUiInterval", data_.killNumberUiInterval);
	json_.Set("killNumberUiSize", data_.killNumberUiSize);
	json_.Set("killNumberUiPos", data_.killNumberUiPos);

	json_.Set("hitRateNumberUiInterval", data_.hitRateNumberUiInterval);
	json_.Set("hitRateNumberUiSize", data_.hitRateNumberUiSize);
	json_.Set("hitRateNumberUiPos", data_.hitRateNumberUiPos);

	json_.Set("selectUiInterval", data_.selectUiInterval);
	json_.Set("clearCountUiTimer", data_.clearCountUiTimer_);
	json_.Save();
}