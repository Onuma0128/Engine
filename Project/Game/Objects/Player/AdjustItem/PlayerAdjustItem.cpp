#include "PlayerAdjustItem.h"

#include "imgui.h"

void PlayerAdjustItem::LoadItems()
{
	/* ============================== Player ============================== */

	playerJson_.Init("Player");
	if (!playerJson_.Load()) {
		playerJson_.Set("clearKill", 1);
		playerJson_.Set("minPlayerClamp", Vector2{ -1.0f,-1.0f });
		playerJson_.Set("maxPlayerClamp", Vector2{ 1.0f,1.0f });
		playerJson_.Set("speed", 1.0f);
		playerJson_.Set("backSpeed", 1.0f);
		playerJson_.Set("pushSpeed", 1.0f);
		playerJson_.Set("avoidTime", 1.0f);
		playerJson_.Set("avoid_speed", 1.0f);
		playerJson_.Set("avoid_velocityY", 1.0f);
		playerJson_.Set("avoid_acceleration", 1.0f);
		playerJson_.Set("avoid_coolTime", 1.0f);
		playerJson_.Set("avoidEffectScale", Vector3{});
		playerJson_.Set("avoidEffectPos", Vector3{});
		playerJson_.Set("avoidEffectAlpha", 1.0f);

		playerJson_.Set("needMachoEffectTime", 1.0f);
		playerJson_.Set("needMachoScale", Vector3{});
		playerJson_.Set("needMachoPos", Vector3{});
		playerJson_.Set("needMachoVarianceScale", Vector3{});
		playerJson_.Set("needMachoVariancePos", Vector3{});
	} else {
		playerData_.isInvincible = false;
		playerData_.clearKill = playerJson_.Get("clearKill", playerData_.clearKill);
		playerData_.minPlayerClamp = playerJson_.Get("minPlayerClamp", playerData_.minPlayerClamp);
		playerData_.maxPlayerClamp = playerJson_.Get("maxPlayerClamp", playerData_.maxPlayerClamp);
		playerData_.speed = playerJson_.Get("speed", playerData_.speed);
		playerData_.backSpeed = playerJson_.Get("backSpeed", playerData_.backSpeed);
		playerData_.pushSpeed = playerJson_.Get("pushSpeed", playerData_.pushSpeed);
		playerData_.avoidTime = playerJson_.Get("avoidTime", playerData_.avoidTime);
		playerData_.avoid_speed = playerJson_.Get("avoid_speed", playerData_.avoid_speed);
		playerData_.avoid_velocityY = playerJson_.Get("avoid_velocityY", playerData_.avoid_velocityY);
		playerData_.avoid_acceleration = playerJson_.Get("avoid_acceleration", playerData_.avoid_acceleration);
		playerData_.avoid_coolTime = playerJson_.Get("avoid_coolTime", playerData_.avoid_coolTime);
		playerData_.avoidEffectScale = playerJson_.Get("avoidEffectScale", playerData_.avoidEffectScale);
		playerData_.avoidEffectPos = playerJson_.Get("avoidEffectPos", playerData_.avoidEffectPos);
		playerData_.avoidEffectAlpha = playerJson_.Get("avoidEffectAlpha", playerData_.avoidEffectAlpha);

		playerData_.needMachoEffectTime = playerJson_.Get("needMachoEffectTime", playerData_.needMachoEffectTime);
		playerData_.needMachoScale = playerJson_.Get("needMachoScale", playerData_.needMachoScale);
		playerData_.needMachoPos = playerJson_.Get("needMachoPos", playerData_.needMachoPos);
		playerData_.needMachoVarianceScale = playerJson_.Get("needMachoVarianceScale", playerData_.needMachoVarianceScale);
		playerData_.needMachoVariancePos = playerJson_.Get("needMachoVariancePos", playerData_.needMachoVariancePos);
	}

	/* ============================== Bullet ============================== */

	bulletJson_.Init("PlayerBullet");
	if (!bulletJson_.Load()) {
		bulletJson_.Set("speed", 1.0f);
		bulletJson_.Set("speed_sp", 1.0f);
		bulletJson_.Set("position", Vector3{});
		bulletJson_.Set("maxChargeCount_sp", 3);
		bulletJson_.Set("reloadStartTime", 1.0f);
		bulletJson_.Set("reloadTime", 1.0f);
		bulletJson_.Set("colliderSize", Vector3{});
		bulletJson_.Set("colliderCenterPosition", Vector3{});
	} else {
		bulletData_.speed = bulletJson_.Get("speed", bulletData_.speed);
		bulletData_.speed_sp = bulletJson_.Get("speed_sp", bulletData_.speed_sp);
		bulletData_.position = bulletJson_.Get("position", bulletData_.position);
		bulletData_.maxChargeCount_sp = bulletJson_.Get("maxChargeCount_sp", bulletData_.maxChargeCount_sp);
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
		preObjectJson_.Set("rayColliderSize", Vector3{});
		preObjectJson_.Set("rayColliderPosition", Vector3{});
	} else {
		preObjectData_.interval = preObjectJson_.Get("interval", preObjectData_.interval);
		preObjectData_.startPosition = preObjectJson_.Get("startPosition", preObjectData_.startPosition);
		preObjectData_.rayColliderSize = preObjectJson_.Get("rayColliderSize", preObjectData_.rayColliderSize);
		preObjectData_.rayColliderPosition = preObjectJson_.Get("rayColliderPosition", preObjectData_.rayColliderPosition);
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

	ImGui::Begin("GameData");
	if (ImGui::TreeNode("PlayerParameters")) {
		ImGui::Separator();

		ImGui::Checkbox("invincible", &playerData_.isInvincible);

		ImGui::PushItemWidth(150);

		if (ImGui::TreeNode("Player")) {

			ImGui::DragInt("clearKill", &playerData_.clearKill);
			ImGui::DragFloat2("minPlayerClamp", &playerData_.minPlayerClamp.x, 0.1f, -1000.0f, 0.0f);
			ImGui::DragFloat2("maxPlayerClamp", &playerData_.maxPlayerClamp.x, 0.1f, 0.0f, 1000.0f);
			ImGui::Separator();
			ImGui::DragFloat("speed", &playerData_.speed, 0.01f);
			ImGui::DragFloat("backSpeed", &playerData_.backSpeed, 0.01f);
			ImGui::DragFloat("pushSpeed", &playerData_.pushSpeed, 0.01f);
			ImGui::Separator();
			ImGui::DragFloat("avoidTime", &playerData_.avoidTime, 0.01f);
			ImGui::DragFloat("avoid_speed", &playerData_.avoid_speed, 0.01f);
			ImGui::DragFloat("avoid_velocityY", &playerData_.avoid_velocityY, 0.01f);
			ImGui::DragFloat("avoid_acceleration", &playerData_.avoid_acceleration, 0.01f);
			ImGui::DragFloat("avoid_coolTime", &playerData_.avoid_coolTime, 0.01f);
			ImGui::DragFloat3("avoidEffectScale", &playerData_.avoidEffectScale.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("avoidEffectPos", &playerData_.avoidEffectPos.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat("avoidEffectAlpha", &playerData_.avoidEffectAlpha, 0.01f);
			ImGui::Separator();
			ImGui::DragFloat("needMachoEffectTime", &playerData_.needMachoEffectTime, 0.01f);
			ImGui::DragFloat3("needMachoScale", &playerData_.needMachoScale.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("needMachoPos", &playerData_.needMachoPos.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("needMachoVarianceScale", &playerData_.needMachoVarianceScale.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("needMachoVariancePos", &playerData_.needMachoVariancePos.x, 0.1f, -10.0f, 10.0f);

			if (ImGui::Button("Save")) {
				playerJson_.Set("clearKill", playerData_.clearKill);
				playerJson_.Set("minPlayerClamp", playerData_.minPlayerClamp);
				playerJson_.Set("maxPlayerClamp", playerData_.maxPlayerClamp);
				playerJson_.Set("speed", playerData_.speed);
				playerJson_.Set("backSpeed", playerData_.backSpeed);
				playerJson_.Set("pushSpeed", playerData_.pushSpeed);
				playerJson_.Set("avoidTime", playerData_.avoidTime);
				playerJson_.Set("avoid_speed", playerData_.avoid_speed);
				playerJson_.Set("avoid_velocityY", playerData_.avoid_velocityY);
				playerJson_.Set("avoid_acceleration", playerData_.avoid_acceleration);
				playerJson_.Set("avoid_coolTime", playerData_.avoid_coolTime);
				playerJson_.Set("avoidEffectScale", playerData_.avoidEffectScale);
				playerJson_.Set("avoidEffectPos", playerData_.avoidEffectPos);
				playerJson_.Set("avoidEffectAlpha", playerData_.avoidEffectAlpha);
				playerJson_.Set("needMachoEffectTime", playerData_.needMachoEffectTime);
				playerJson_.Set("needMachoScale", playerData_.needMachoScale);
				playerJson_.Set("needMachoPos", playerData_.needMachoPos);
				playerJson_.Set("needMachoVarianceScale", playerData_.needMachoVarianceScale);
				playerJson_.Set("needMachoVariancePos", playerData_.needMachoVariancePos);

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
			ImGui::DragInt("maxChargeCount_sp", &bulletData_.maxChargeCount_sp, 1, 0, 100);
			ImGui::DragFloat("reloadStartTime", &bulletData_.reloadStartTime, 0.01f);
			ImGui::DragFloat("reloadTime", &bulletData_.reloadTime, 0.01f);
			ImGui::DragFloat3("colliderSize", &bulletData_.colliderSize.x, 0.01f);
			ImGui::DragFloat3("colliderPosition", &bulletData_.colliderPosition.x, 0.01f);
			if (ImGui::Button("Save")) {
				bulletJson_.Set("speed", bulletData_.speed);
				bulletJson_.Set("speed_sp", bulletData_.speed_sp);
				bulletJson_.Set("position", bulletData_.position);
				bulletJson_.Set("maxChargeCount_sp", bulletData_.maxChargeCount_sp);
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
			ImGui::DragFloat3("rayColliderSize", &preObjectData_.rayColliderSize.x, 0.01f);
			ImGui::DragFloat3("rayColliderPosition", &preObjectData_.rayColliderPosition.x, 0.01f);
			if (ImGui::Button("Save")) {
				preObjectJson_.Set("interval", preObjectData_.interval);
				preObjectJson_.Set("startPosition", preObjectData_.startPosition);
				preObjectJson_.Set("rayColliderSize", preObjectData_.rayColliderSize);
				preObjectJson_.Set("rayColliderPosition", preObjectData_.rayColliderPosition);
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
		ImGui::TreePop();
	}
	ImGui::Separator();

	ImGui::End();
}
