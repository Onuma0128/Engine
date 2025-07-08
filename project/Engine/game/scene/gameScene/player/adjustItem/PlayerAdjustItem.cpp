#include "PlayerAdjustItem.h"

#include "imgui.h"

void PlayerAdjustItem::LoadItems()
{
	/* ============================== Player ============================== */

	playerJson_.Init("Player");
	if (!playerJson_.Load()) {
		playerJson_.Set("speed", 1.0f);
		playerJson_.Set("avoidTime", 1.0f);
		playerJson_.Set("avoid_speed", 1.0f);
		playerJson_.Set("avoid_velocityY", 1.0f);
		playerJson_.Set("avoid_acceleration", 1.0f);
	} else {
		playerData_.speed = playerJson_.Get("speed", playerData_.speed);
		playerData_.avoidTime = playerJson_.Get("avoidTime", playerData_.avoidTime);
		playerData_.avoid_speed = playerJson_.Get("avoid_speed", playerData_.avoid_speed);
		playerData_.avoid_velocityY = playerJson_.Get("avoid_velocityY", playerData_.avoid_velocityY);
		playerData_.avoid_acceleration = playerJson_.Get("avoid_acceleration", playerData_.avoid_acceleration);
	}

	/* ============================== Bullet ============================== */

	bulletJson_.Init("PlayerBullet");
	if (!bulletJson_.Load()) {
		bulletJson_.Set("speed", 1.0f);
		bulletJson_.Set("speed_sp", 1.0f);
		bulletJson_.Set("position", Vector3{});
		bulletJson_.Set("reloadStartTime", 1.0f);
		bulletJson_.Set("reloadTime", 1.0f);
		bulletJson_.Set("colliderSize", Vector3{});
		bulletJson_.Set("colliderCenterPosition", Vector3{});
	} else {
		bulletData_.speed = bulletJson_.Get("speed", bulletData_.speed);
		bulletData_.speed_sp = bulletJson_.Get("speed_sp", bulletData_.speed_sp);
		bulletData_.position = bulletJson_.Get("position", bulletData_.position);
		bulletData_.reloadStartTime = bulletJson_.Get("reloadStartTime", bulletData_.reloadStartTime);
		bulletData_.reloadTime = bulletJson_.Get("reloadTime", bulletData_.reloadTime);
		bulletData_.colliderSize = bulletJson_.Get("colliderSize", bulletData_.colliderSize);
		bulletData_.colliderPosition = bulletJson_.Get("colliderPosition", bulletData_.colliderPosition);
	}

	/* ============================== PredictionObject ============================== */
	
	preObjectJson_.Init("PlayerPredictionObject");
	if (!preObjectJson_.Load()) {
		preObjectJson_.Set("interval", 1.0f);
		preObjectJson_.Set("startPosition", Vector3{});
	} else {
		preObjectData_.interval = preObjectJson_.Get("interval", preObjectData_.interval);
		preObjectData_.startPosition = preObjectJson_.Get("startPosition", preObjectData_.startPosition);
	}

	/* ============================== BulletUI ============================== */

	bulletUIJson_.Init("PlayerBulletUI");
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
	/* ============================== Player ============================== */

	ImGui::Begin("PlayerParameters");

	if (ImGui::TreeNode("Player")) {

		ImGui::DragFloat("speed", &playerData_.speed, 0.01f);
		ImGui::Separator();
		ImGui::DragFloat("avoidTime", &playerData_.avoidTime, 0.01f);
		ImGui::DragFloat("avoid_speed", &playerData_.avoid_speed, 0.01f);
		ImGui::DragFloat("avoid_velocityY", &playerData_.avoid_velocityY, 0.01f);
		ImGui::DragFloat("avoid_acceleration", &playerData_.avoid_acceleration, 0.01f);

		if (ImGui::Button("Save")) {
			playerJson_.Set("speed", playerData_.speed);
			playerJson_.Set("avoidTime", playerData_.avoidTime);
			playerJson_.Set("avoid_speed", playerData_.avoid_speed);
			playerJson_.Set("avoid_velocityY", playerData_.avoid_velocityY);
			playerJson_.Set("avoid_acceleration", playerData_.avoid_acceleration);
			playerJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== Bullet ============================== */

	if (ImGui::TreeNode("Bullet")) {

		ImGui::DragFloat("speed", &bulletData_.speed, 0.01f);
		ImGui::DragFloat("speed_sp", &bulletData_.speed_sp, 0.01f);
		ImGui::DragFloat3("position", &bulletData_.position.x, 0.01f);
		ImGui::DragFloat("reloadStartTime", &bulletData_.reloadStartTime, 0.01f);
		ImGui::DragFloat("reloadTime", &bulletData_.reloadTime, 0.01f);
		ImGui::DragFloat3("colliderSize", &bulletData_.colliderSize.x, 0.01f);
		ImGui::DragFloat3("colliderPosition", &bulletData_.colliderPosition.x, 0.01f);
		if (ImGui::Button("Save")) {
			bulletJson_.Set("speed", bulletData_.speed);
			bulletJson_.Set("speed_sp", bulletData_.speed_sp);
			bulletJson_.Set("position", bulletData_.position);
			bulletJson_.Set("reloadStartTime", bulletData_.reloadStartTime);
			bulletJson_.Set("reloadTime", bulletData_.reloadTime);
			bulletJson_.Set("colliderSize", bulletData_.colliderSize);
			bulletJson_.Set("colliderPosition", bulletData_.colliderPosition);
			bulletJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== PredictionObject ============================== */

	if (ImGui::TreeNode("PredictionObject")) {

		ImGui::DragFloat("interval", &preObjectData_.interval, 0.01f);
		ImGui::DragFloat3("startPosition", &preObjectData_.startPosition.x, 0.01f);
		if (ImGui::Button("Save")) {
			preObjectJson_.Set("interval", preObjectData_.interval);
			preObjectJson_.Set("startPosition", preObjectData_.startPosition);
			preObjectJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== BulletUI ============================== */

	if (ImGui::TreeNode("BulletUI")) {

		ImGui::DragFloat2("size", &bulletUIData_.size.x);
		ImGui::DragFloat("startPosition", &bulletUIData_.startPosition);
		ImGui::DragFloat2("position", &bulletUIData_.position.x);
		if (ImGui::Button("Save")) {
			bulletUIJson_.Set("size", bulletUIData_.size);
			bulletUIJson_.Set("position", bulletUIData_.position);
			bulletUIJson_.Set("startPosition", bulletUIData_.startPosition);
			bulletUIJson_.Save();
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
