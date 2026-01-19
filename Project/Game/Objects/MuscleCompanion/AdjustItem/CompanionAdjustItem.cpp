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
		mainJson_.Set("objectScale", 0.0f);
		mainJson_.Set("searchUpdateTime", 0.0f);
		mainJson_.Set("searchCancelDistance", 0.0f);
		mainJson_.Set("colliderSize", 0.0f);
		mainJson_.Set("colliderOffset", Vector3{});

		mainJson_.Set("blinkingHP", 0);
		mainJson_.Set("blinkingColor", 0.0f);
		mainJson_.Set("blinkingTimeScale", 0.0f);
		mainJson_.Set("distanceToAlly", 0.0f);
		mainJson_.Set("followerColliderSize", 0.0f);
		mainJson_.Set("debugSpline", false);
		mainJson_.Set("clearStateTime", 0.0f);
	} else {
		mainData_.maxHP = mainJson_.Get("maxHP", mainData_.maxHP);
		mainData_.speed = mainJson_.Get("speed", mainData_.speed);
		mainData_.hitPushSpeed = mainJson_.Get("hitPushSpeed", mainData_.hitPushSpeed);
		mainData_.objectScale = mainJson_.Get("objectScale", mainData_.objectScale);
		mainData_.colliderSize = mainJson_.Get("colliderSize", mainData_.colliderSize);
		mainData_.colliderOffset = mainJson_.Get("colliderOffset", mainData_.colliderOffset);

		mainData_.blinkingHP = mainJson_.Get("blinkingHP", mainData_.blinkingHP);
		mainData_.blinkingColor = mainJson_.Get("blinkingColor", mainData_.blinkingColor);
		mainData_.blinkingTimeScale = mainJson_.Get("blinkingTimeScale", mainData_.blinkingTimeScale);

		mainData_.searchUpdateTime = mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
		mainData_.searchCancelDistance = mainJson_.Get("searchCancelDistance", mainData_.searchCancelDistance);
		mainData_.distanceToAlly = mainJson_.Get("distanceToAlly", mainData_.distanceToAlly);
		mainData_.followerColliderSize = mainJson_.Get("followerColliderSize", mainData_.followerColliderSize);
		mainData_.debugSpline = mainJson_.Get("debugSpline", mainData_.debugSpline);
		mainData_.clearStateTime = mainJson_.Get("clearStateTime", mainData_.clearStateTime);
	}
	mainData_.debugSpline = false;

	/* ============================== Dash ============================== */
	dashJson_.Init("CompanionDash");
	if(!dashJson_.Load()) {
		dashJson_.Set("dashSpeed", 0.0f);
		dashJson_.Set("dashTargetDistance", 0.0f);
		dashJson_.Set("dashColliderScale", 1.0f);
		dashJson_.Set("searchDashColliderScale", 1.0f);
		dashJson_.Set("pushUpTime", 1.0f);
	} else {
		dashData_.dashSpeed = dashJson_.Get("dashSpeed", dashData_.dashSpeed);
		dashData_.dashTargetDistance = dashJson_.Get("dashTargetDistance", dashData_.dashTargetDistance);
		dashData_.dashColliderScale = dashJson_.Get("dashColliderScale", dashData_.dashColliderScale);
		dashData_.searchDashColliderScale = dashJson_.Get("searchDashColliderScale", dashData_.searchDashColliderScale); dashData_.searchDashColliderScale = dashJson_.Get("searchDashColliderScale", dashData_.searchDashColliderScale);
		dashData_.pushUpTime = dashJson_.Get("pushUpTime", dashData_.pushUpTime);
	}

	/* ============================== Attack ============================== */
	attackJson_.Init("CompanionAttack");
	if (!attackJson_.Load()) {
		attackJson_.Set("shakePowerHigh", 0.0f);
		attackJson_.Set("shakePowerLow", 0.0f);
		attackJson_.Set("attackColliderSize", 0.0f);
		attackJson_.Set("attackColliderOffset", Vector3{});
		attackJson_.Set("firstDashTime", 0.0f);
		attackJson_.Set("attackStartupTime", 0.0f);
		attackJson_.Set("attackActiveTime", 0.0f);
		attackJson_.Set("attackRecoveryTime", 0.0f);
	} else {
		attackData_.shakePowerHigh = attackJson_.Get("shakePowerHigh", attackData_.shakePowerHigh);
		attackData_.shakePowerLow = attackJson_.Get("shakePowerLow", attackData_.shakePowerLow);
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

		knockbackJson_.Set("shieldKnockbackDire", 0.0f);
		knockbackJson_.Set("shieldKnockbackSpeed", 0.0f);
		knockbackJson_.Set("shieldKnockbackTime", 1.0f);
	} else {
		knockbackData_.knockbackSpeed = knockbackJson_.Get("knockbackSpeed", knockbackData_.knockbackSpeed);
		knockbackData_.knockbackTime = knockbackJson_.Get("knockbackTime", knockbackData_.knockbackTime);

		knockbackData_.shieldKnockbackDire = knockbackJson_.Get("shieldKnockbackDire", knockbackData_.shieldKnockbackDire);
		knockbackData_.shieldKnockbackSpeed = knockbackJson_.Get("shieldKnockbackSpeed", knockbackData_.shieldKnockbackSpeed);
		knockbackData_.shieldKnockbackTime = knockbackJson_.Get("shieldKnockbackTime", knockbackData_.shieldKnockbackTime);
	}

	/* ============================== Effect ============================== */
	effectJson_.Init("CompanionEffect");
	if (!effectJson_.Load()) {
		effectJson_.Set("nextArrowAnimaTime", 0.0f);
		effectJson_.Set("nextArrowScale", 1.0f);
		effectJson_.Set("nextArrowPosition", 1.0f);
		effectJson_.Set("nextArrowVarianceScale", 1.0f);
		effectJson_.Set("nextArrowVariancePosition", 1.0f);
		effectJson_.Set("lerpSpeed", 1.0f);
	} else {
		effectData_.nextArrowAnimaTime = effectJson_.Get("nextArrowAnimaTime", effectData_.nextArrowAnimaTime);
		effectData_.nextArrowScale = effectJson_.Get("nextArrowScale", effectData_.nextArrowScale);
		effectData_.nextArrowPosition = effectJson_.Get("nextArrowPosition", effectData_.nextArrowPosition);
		effectData_.nextArrowVarianceScale = effectJson_.Get("nextArrowVarianceScale", effectData_.nextArrowVarianceScale);
		effectData_.nextArrowVariancePosition = effectJson_.Get("nextArrowVariancePosition", effectData_.nextArrowVariancePosition);
		effectData_.lerpSpeed = effectJson_.Get("lerpSpeed", effectData_.lerpSpeed);
	}

	/* ============================== SeVolume ============================== */
	seVolumeJson_.Init("CompanionSeVolume");
	if (!seVolumeJson_.Load()) {
		seVolumeJson_.Set("dashHit", 0.0f);
		seVolumeJson_.Set("footsteps", 1.0f);
		seVolumeJson_.Set("getDamage", 0.0f);
		seVolumeJson_.Set("giveDamage", 1.0f);
		seVolumeJson_.Set("set", 0.0f);
		seVolumeJson_.Set("shot", 1.0f);
		seVolumeJson_.Set("clear", 1.0f);
		seVolumeJson_.Set("shield", 0.0f);
		seVolumeJson_.Set("powerUp", 1.0f);
	} else {
		seVolumeData_.dashHit = seVolumeJson_.Get("dashHit", seVolumeData_.dashHit);
		seVolumeData_.footsteps = seVolumeJson_.Get("footsteps", seVolumeData_.footsteps);
		seVolumeData_.getDamage = seVolumeJson_.Get("getDamage", seVolumeData_.getDamage);
		seVolumeData_.giveDamage = seVolumeJson_.Get("giveDamage", seVolumeData_.giveDamage);
		seVolumeData_.set = seVolumeJson_.Get("set", seVolumeData_.set);
		seVolumeData_.shot = seVolumeJson_.Get("shot", seVolumeData_.shot);
		seVolumeData_.clear = seVolumeJson_.Get("clear", seVolumeData_.clear);
		seVolumeData_.shield = seVolumeJson_.Get("shield", seVolumeData_.shield);
		seVolumeData_.powerUp = seVolumeJson_.Get("powerUp", seVolumeData_.powerUp);
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
			ImGui::DragFloat("objectScale", &mainData_.objectScale, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f, 0.0f, 100.0f);

			ImGui::DragInt("blinkingHP", &mainData_.blinkingHP, 1, 1, 1000);
			ImGui::DragFloat("blinkingColor", &mainData_.blinkingColor, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("blinkingTimeScale", &mainData_.blinkingTimeScale, 0.01f, 0.0f, 10.0f);

			ImGui::DragFloat("distanceToAlly", &mainData_.distanceToAlly, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("followerColliderSize", &mainData_.followerColliderSize, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("searchCancelDistance", &mainData_.searchCancelDistance, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("clearStateTime", &mainData_.clearStateTime, 0.01f, 0.0f, 100.0f);
			ImGui::Checkbox("debugSpline", &mainData_.debugSpline);
			// セーブボタン
			if (ImGui::Button("Save")) {
				mainJson_.Set("maxHP", mainData_.maxHP);
				mainJson_.Set("speed", mainData_.speed);
				mainJson_.Set("hitPushSpeed", mainData_.hitPushSpeed);
				mainJson_.Set("objectScale", mainData_.objectScale);
				mainJson_.Set("colliderSize", mainData_.colliderSize);
				mainJson_.Set("colliderOffset", mainData_.colliderOffset);
				mainJson_.Set("blinkingHP", mainData_.blinkingHP);
				mainJson_.Set("blinkingColor", mainData_.blinkingColor);
				mainJson_.Set("blinkingTimeScale", mainData_.blinkingTimeScale);
				mainJson_.Set("distanceToAlly", mainData_.distanceToAlly);
				mainJson_.Set("followerColliderSize", mainData_.followerColliderSize);
				mainJson_.Set("searchUpdateTime", mainData_.searchUpdateTime);
				mainJson_.Set("searchCancelDistance", mainData_.searchCancelDistance);
				mainJson_.Set("clearStateTime", mainData_.clearStateTime);
				mainJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		// ノックバック項目
		if (ImGui::TreeNode("Knockback")) {
			ImGui::DragFloat("knockbackSpeed", &knockbackData_.knockbackSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("knockbackTime", &knockbackData_.knockbackTime, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat3("shieldKnockbackDire", &knockbackData_.shieldKnockbackDire.x, 0.01f, -100.0f, 100.0f);
			ImGui::DragFloat("shieldKnockbackSpeed", &knockbackData_.shieldKnockbackSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("shieldKnockbackTime", &knockbackData_.shieldKnockbackTime, 0.01f, 0.1f, 10.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				knockbackJson_.Set("knockbackSpeed", knockbackData_.knockbackSpeed);
				knockbackJson_.Set("knockbackTime", knockbackData_.knockbackTime);
				knockbackJson_.Set("shieldKnockbackDire", knockbackData_.shieldKnockbackDire);
				knockbackJson_.Set("shieldKnockbackSpeed", knockbackData_.shieldKnockbackSpeed);
				knockbackJson_.Set("shieldKnockbackTime", knockbackData_.shieldKnockbackTime);
				knockbackJson_.Save();
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		// ダッシュ項目
		if (ImGui::TreeNode("Dash")) {
			ImGui::DragFloat("dashSpeed", &dashData_.dashSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("dashTargetDistance", &dashData_.dashTargetDistance, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("dashColliderScale", &dashData_.dashColliderScale, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("searchDashColliderScale", &dashData_.searchDashColliderScale, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("pushUpTime", &dashData_.pushUpTime, 0.01f, 0.1f, 10.0f);
			// セーブボタン
			if(ImGui::Button("Save")) {
				dashJson_.Set("dashSpeed", dashData_.dashSpeed);
				dashJson_.Set("dashTargetDistance", dashData_.dashTargetDistance);
				dashJson_.Set("dashColliderScale", dashData_.dashColliderScale);
				dashJson_.Set("searchDashColliderScale", dashData_.searchDashColliderScale);
				dashJson_.Set("pushUpTime", dashData_.pushUpTime);
				dashJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		// 攻撃項目
		if (ImGui::TreeNode("Attack")) {
			ImGui::DragFloat("shakePowerHigh", &attackData_.shakePowerHigh, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("shakePowerLow", &attackData_.shakePowerLow, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("attackColliderSize", &attackData_.attackColliderSize, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat3("attackColliderOffset", &attackData_.attackColliderOffset.x, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("firstDashTime", &attackData_.firstDashTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackStartupTime", &attackData_.attackStartupTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackActiveTime", &attackData_.attackActiveTime, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("attackRecoveryTime", &attackData_.attackRecoveryTime, 0.01f, 0.0f, 10.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				attackJson_.Set("shakePowerHigh", attackData_.shakePowerHigh);
				attackJson_.Set("shakePowerLow", attackData_.shakePowerLow);
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
		ImGui::Separator();
		// エフェクトの項目
		if (ImGui::TreeNode("Effect")) {
			ImGui::DragFloat("nextArrowAnimaTime", &effectData_.nextArrowAnimaTime, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat3("nextArrowScale", &effectData_.nextArrowScale.x, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat3("nextArrowPosition", &effectData_.nextArrowPosition.x, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat3("nextArrowVarianceScale", &effectData_.nextArrowVarianceScale.x, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat3("nextArrowVariancePosition", &effectData_.nextArrowVariancePosition.x, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("lerpSpeed", &effectData_.lerpSpeed, 0.01f, 0.0f, 100.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				effectJson_.Set("nextArrowAnimaTime", effectData_.nextArrowAnimaTime);
				effectJson_.Set("nextArrowScale", effectData_.nextArrowScale);
				effectJson_.Set("nextArrowPosition", effectData_.nextArrowPosition);
				effectJson_.Set("nextArrowVarianceScale", effectData_.nextArrowVarianceScale);
				effectJson_.Set("nextArrowVariancePosition", effectData_.nextArrowVariancePosition);
				effectJson_.Set("lerpSpeed", effectData_.lerpSpeed);
				effectJson_.Save();
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		// 効果音の項目
		if (ImGui::TreeNode("SeVolume")) {
			ImGui::DragFloat("dashHit", &seVolumeData_.dashHit, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("footsteps", &seVolumeData_.footsteps, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("getDamage", &seVolumeData_.getDamage, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("giveDamage", &seVolumeData_.giveDamage, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("set", &seVolumeData_.set, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("shot", &seVolumeData_.shot, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("clear", &seVolumeData_.clear, 0.01f, 0.1f, 10.0f);
			ImGui::DragFloat("shield", &seVolumeData_.shield, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("powerUp", &seVolumeData_.powerUp, 0.01f, 0.1f, 10.0f);
			// セーブボタン
			if (ImGui::Button("Save")) {
				seVolumeJson_.Set("dashHit", seVolumeData_.dashHit);
				seVolumeJson_.Set("footsteps", seVolumeData_.footsteps);
				seVolumeJson_.Set("getDamage", seVolumeData_.getDamage);
				seVolumeJson_.Set("giveDamage", seVolumeData_.giveDamage);
				seVolumeJson_.Set("set", seVolumeData_.set);
				seVolumeJson_.Set("shot", seVolumeData_.shot);
				seVolumeJson_.Set("clear", seVolumeData_.clear);
				seVolumeJson_.Set("shield", seVolumeData_.shield);
				seVolumeJson_.Set("powerUp", seVolumeData_.powerUp);
				seVolumeJson_.Save();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();
}