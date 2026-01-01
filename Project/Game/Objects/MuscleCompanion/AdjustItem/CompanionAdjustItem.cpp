#include "CompanionAdjustItem.h"

#include "imgui.h"

void CompanionAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */
	mainJson_.Init("CompanionMain");
	if (!mainJson_.Load()) {
		mainJson_.Set("maxHP", 1);
		mainJson_.Set("speed", 0.0f);
		mainJson_.Set("hitPushSpeed", 0.0f);
		mainJson_.Set("searchUpdateTime", 0.0f);
		mainJson_.Set("searchCancelDistance", 0.0f);
		mainJson_.Set("colliderSize", 0.0f);
		mainJson_.Set("colliderOffset", Vector3{});
		mainJson_.Set("distanceToAlly", 0.0f);
		mainJson_.Set("followerColliderSize", 0.0f);
		mainJson_.Set("debugSpline", false);
	} else {
		mainData_.maxHP = mainJson_.Get("maxHP", mainData_.maxHP);
		mainData_.speed = mainJson_.Get("speed", mainData_.speed);
		mainData_.hitPushSpeed = mainJson_.Get("hitPushSpeed", mainData_.hitPushSpeed);
		mainData_.colliderSize = mainJson_.Get("colliderSize", mainData_.colliderSize);
		mainData_.colliderOffset = mainJson_.Get("colliderOffset", mainData_.colliderOffset);
		mainData_.searchUpdateTime = mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
		mainData_.searchCancelDistance = mainJson_.Get("searchCancelDistance", mainData_.searchCancelDistance);
		mainData_.distanceToAlly = mainJson_.Get("distanceToAlly", mainData_.distanceToAlly);
		mainData_.followerColliderSize = mainJson_.Get("followerColliderSize", mainData_.followerColliderSize);
		mainData_.debugSpline = mainJson_.Get("debugSpline", mainData_.debugSpline);
	}
	mainData_.debugSpline = false;

	/* ============================== Dash ============================== */
	dashJson_.Init("CompanionDash");
	if(!dashJson_.Load()) {
		dashJson_.Set("dashSpeed", 0.0f);
		dashJson_.Set("dashColliderScale", 1.0f);
	} else {
		dashData_.dashSpeed = dashJson_.Get("dashSpeed", dashData_.dashSpeed);
		dashData_.dashColliderScale = dashJson_.Get("dashColliderScale", dashData_.dashColliderScale);
	}

	/* ============================== Attack ============================== */
	attackJson_.Init("CompanionAttack");
	if (!attackJson_.Load()) {
		attackJson_.Set("attackColliderSize", 0.0f);
		attackJson_.Set("attackColliderOffset", Vector3{});
		attackJson_.Set("firstDashTime", 0.0f);
		attackJson_.Set("attackStartupTime", 0.0f);
		attackJson_.Set("attackActiveTime", 0.0f);
		attackJson_.Set("attackRecoveryTime", 0.0f);
	} else {
		attackData_.attackColliderSize = attackJson_.Get("attackColliderSize", attackData_.attackColliderSize);
		attackData_.attackColliderOffset = attackJson_.Get("attackColliderOffset", attackData_.attackColliderOffset);
		attackData_.firstDashTime = attackJson_.Get("firstDashTime", attackData_.firstDashTime);
		attackData_.attackStartupTime = attackJson_.Get("attackStartupTime", attackData_.attackStartupTime);
		attackData_.attackActiveTime = attackJson_.Get("attackActiveTime", attackData_.attackActiveTime);
		attackData_.attackRecoveryTime = attackJson_.Get("attackRecoveryTime", attackData_.attackRecoveryTime);
	}

	/* ============================== Knockback ============================== */
	knockbackJson_.Init("CompanionKnockback");
	if (!knockbackJson_.Load()) {
		knockbackJson_.Set("knockbackSpeed", 0.0f);
		knockbackJson_.Set("knockbackTime", 1.0f);
	} else {
		knockbackData_.knockbackSpeed = knockbackJson_.Get("knockbackSpeed", knockbackData_.knockbackSpeed);
		knockbackData_.knockbackTime = knockbackJson_.Get("knockbackTime", knockbackData_.knockbackTime);
	}
}

void CompanionAdjustItem::Editor()
{
	ImGui::Begin("GameData");
	if (ImGui::TreeNode("CompanionParameters")) {
		ImGui::Separator();
		ImGui::PushItemWidth(150);

		// メイン項目
		if (ImGui::TreeNode("Main")) {
			ImGui::DragInt("maxHP", &mainData_.maxHP, 1, 1, 1000);
			ImGui::DragFloat("speed", &mainData_.speed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("hitPushSpeed", &mainData_.hitPushSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("distanceToAlly", &mainData_.distanceToAlly, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("followerColliderSize", &mainData_.followerColliderSize, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("searchCancelDistance", &mainData_.searchCancelDistance, 0.01f, 0.0f, 100.0f);
			ImGui::Checkbox("debugSpline", &mainData_.debugSpline);
			// セーブボタン
			if (ImGui::Button("Save")) {
				mainJson_.Set("maxHP", mainData_.maxHP);
				mainJson_.Set("speed", mainData_.speed);
				mainJson_.Set("hitPushSpeed", mainData_.hitPushSpeed);
				mainJson_.Set("colliderSize", mainData_.colliderSize);
				mainJson_.Set("colliderOffset", mainData_.colliderOffset);
				mainJson_.Set("distanceToAlly", mainData_.distanceToAlly);
				mainJson_.Set("followerColliderSize", mainData_.followerColliderSize);
				mainJson_.Set("searchUpdateTime", mainData_.searchUpdateTime);
				mainJson_.Set("searchCancelDistance", mainData_.searchCancelDistance);
				mainJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		// ノックバック項目
		if (ImGui::TreeNode("Knockback")) {
			ImGui::DragFloat("knockbackSpeed", &knockbackData_.knockbackSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("knockbackTime", &knockbackData_.knockbackTime, 0.01f, 0.1f, 10.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				knockbackJson_.Set("knockbackSpeed", knockbackData_.knockbackSpeed);
				knockbackJson_.Set("knockbackTime", knockbackData_.knockbackTime);
				knockbackJson_.Save();
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		// ダッシュ項目
		if (ImGui::TreeNode("Dash")) {
			ImGui::DragFloat("dashSpeed", &dashData_.dashSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("dashColliderScale", &dashData_.dashColliderScale, 0.01f, 0.1f, 10.0f);
			// セーブボタン
			if(ImGui::Button("Save")) {
				dashJson_.Set("dashSpeed", dashData_.dashSpeed);
				dashJson_.Set("dashColliderScale", dashData_.dashColliderScale);
				dashJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		// 攻撃項目
		if (ImGui::TreeNode("Attack")) {
			ImGui::DragFloat("attackColliderSize", &attackData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat3("attackColliderOffset", &attackData_.attackColliderOffset.x, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("firstDashTime", &attackData_.firstDashTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackStartupTime", &attackData_.attackStartupTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackActiveTime", &attackData_.attackActiveTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackRecoveryTime", &attackData_.attackRecoveryTime, 0.01f, 0.0f, 10.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				attackJson_.Set("attackColliderSize", attackData_.attackColliderSize);
				attackJson_.Set("attackColliderOffset", attackData_.attackColliderOffset);
				attackJson_.Set("firstDashTime", attackData_.firstDashTime);
				attackJson_.Set("attackStartupTime", attackData_.attackStartupTime);
				attackJson_.Set("attackActiveTime", attackData_.attackActiveTime);
				attackJson_.Set("attackRecoveryTime", attackData_.attackRecoveryTime);
				attackJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}