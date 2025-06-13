#include "PlayerAdjustItem.h"

#include "imgui.h"

void PlayerAdjustItem::LoadItems()
{
	// 弾UIの読み込み
	bulletUIJson_.Init("bulletUI");
	if (!bulletUIJson_.Load()) {
		bulletUIJson_.Set("size", Vector2{});
		bulletUIJson_.Set("position", Vector2{});
		bulletUIJson_.Set("startPosition", 0.0f);
	} else {
		bulletUIData_.size = bulletUIJson_.Get("size", bulletUIData_.size);
		bulletUIData_.position = bulletUIJson_.Get("position", bulletUIData_.position);
		bulletUIData_.startPosition = bulletUIJson_.Get("startPosition", bulletUIData_.startPosition);
	}
}

void PlayerAdjustItem::Editor()
{
	// 弾UIのImGui
	ImGui::Begin("playerBulletUI");
	ImGui::DragFloat2("size", &bulletUIData_.size.x);
	ImGui::DragFloat("startPosition", &bulletUIData_.startPosition);
	ImGui::DragFloat2("position", &bulletUIData_.position.x);
	if (ImGui::Button("Save")) {
		bulletUIJson_.Set("size", bulletUIData_.size);
		bulletUIJson_.Set("position", bulletUIData_.position);
		bulletUIJson_.Set("startPosition", bulletUIData_.startPosition);
		bulletUIJson_.Save();
	}
	ImGui::End();
}
