#include "EnemyAdjustItem.h"

#include "imgui.h"

void EnemyAdjustItem::LoadItems()
{
	/* ============================== Melee ============================== */

	meleeJson_.Init("EnemyMelee");
	if (!meleeJson_.Load()) {
		SetJsonParameter(meleeJson_);
		meleeJson_.Set("colliderSize", 0.0f);
		meleeJson_.Set("colliderOffset", Vector3{});
	} else {
		meleeData_.tempData = GetTemplateData(meleeJson_);
		meleeData_.colliderSize = meleeJson_.Get("colliderSize", meleeData_.colliderSize);
		meleeData_.colliderOffset = meleeJson_.Get("colliderOffset", meleeData_.colliderOffset);
	}

	/* ============================== Ranged ============================== */

	rangedJson_.Init("EnemyRanged");
	if (!rangedJson_.Load()) {
		SetJsonParameter(rangedJson_);
		rangedJson_.Set("bulletSpeed", 0.0f);
		rangedJson_.Set("colliderSize", Vector3{});
		rangedJson_.Set("colliderOffset", Vector3{});
	} else {
		rangedData_.tempData = GetTemplateData(rangedJson_);
		rangedData_.bulletSpeed = rangedJson_.Get("bulletSpeed", rangedData_.bulletSpeed);
		rangedData_.colliderSize = rangedJson_.Get("colliderSize", rangedData_.colliderSize);
		rangedData_.colliderOffset = rangedJson_.Get("colliderOffset", rangedData_.colliderOffset);
	}

	/* ============================== ShieldBearer ============================== */

	shieldBearerJson_.Init("EnemyShieldBearer");
	if (!shieldBearerJson_.Load()) {
		SetJsonParameter(shieldBearerJson_);
	} else {
		shieldBearerData_.tempData = GetTemplateData(shieldBearerJson_);
	}

	/* ============================== RangedElite ============================== */

	rangedEliteJson_.Init("EnemyRangedElite");
	if (!rangedEliteJson_.Load()) {
		SetJsonParameter(rangedEliteJson_);
		rangedEliteJson_.Set("bulletSpeed", 0.0f);
	} else {
		rangedEliteData_.tempData = GetTemplateData(rangedEliteJson_);
		rangedEliteData_.bulletSpeed = rangedEliteJson_.Get("bulletSpeed", rangedEliteData_.bulletSpeed);
	}
}

void EnemyAdjustItem::Editor()
{
	/* ============================== Melee ============================== */

	ImGui::Begin("EnemyParameters");

	if (ImGui::TreeNode("Melee")) {

		SetImGuiData(meleeJson_, meleeData_.tempData);
		ImGui::DragFloat("colliderSize", &meleeData_.colliderSize, 0.01f);
		ImGui::DragFloat3("colliderOffset", &meleeData_.colliderOffset.x, 0.01f);
		if (ImGui::Button("Save")) {
			SetTemplateData(meleeJson_, meleeData_.tempData);
			meleeJson_.Set("colliderSize", meleeData_.colliderSize);
			meleeJson_.Set("colliderOffset", meleeData_.colliderOffset);
			meleeJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== Ranged ============================== */

	if (ImGui::TreeNode("Ranged")) {

		SetImGuiData(rangedJson_, rangedData_.tempData);
		ImGui::DragFloat("bulletSpeed", &rangedData_.bulletSpeed, 0.01f);
		ImGui::DragFloat3("colliderSize", &rangedData_.colliderSize.x, 0.01f);
		ImGui::DragFloat3("colliderOffset", &rangedData_.colliderOffset.x, 0.01f);
		if (ImGui::Button("Save")) {
			SetTemplateData(rangedJson_, rangedData_.tempData);
			rangedJson_.Set("bulletSpeed", rangedData_.bulletSpeed);
			rangedJson_.Set("colliderSize", rangedData_.colliderSize);
			rangedJson_.Set("colliderOffset", rangedData_.colliderOffset);
			rangedJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== ShieldBearer ============================== */

	if (ImGui::TreeNode("ShieldBearer")) {

		SetImGuiData(shieldBearerJson_, shieldBearerData_.tempData);
		if (ImGui::Button("Save")) {
			SetTemplateData(shieldBearerJson_, shieldBearerData_.tempData);
			shieldBearerJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== RangedElite ============================== */

	if (ImGui::TreeNode("RangedElite")) {

		SetImGuiData(rangedEliteJson_, rangedEliteData_.tempData);
		ImGui::DragFloat("bulletSpeed", &rangedEliteData_.bulletSpeed, 0.01f);
		if (ImGui::Button("Save")) {
			SetTemplateData(rangedEliteJson_, rangedEliteData_.tempData);
			rangedEliteJson_.Set("bulletSpeed", rangedEliteData_.bulletSpeed);
			rangedEliteJson_.Save();
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void EnemyAdjustItem::SetJsonParameter(JsonFunction json)
{
	json.Set("speed", 0.0f);
	json.Set("attackDistance", 0.0f);
	json.Set("attackStartupTime", 0.0f);
	json.Set("attackActiveTime", 0.0f);
	json.Set("attackRecoveryTime", 0.0f);
	json.Set("attackCoolTime", 0.0f);
}

EnemyTemplate EnemyAdjustItem::GetTemplateData(JsonFunction json)
{
	EnemyTemplate data{};
	data.speed = json.Get("speed", data.speed);
	data.attackDistance = json.Get("attackDistance", data.attackDistance);
	data.attackStartupTime = json.Get("attackStartupTime", data.attackStartupTime);
	data.attackActiveTime = json.Get("attackActiveTime", data.attackActiveTime);
	data.attackRecoveryTime = json.Get("attackRecoveryTime", data.attackRecoveryTime);
	data.attackCoolTime = json.Get("attackCoolTime", data.attackCoolTime);
	return data;
}

void EnemyAdjustItem::SetImGuiData(JsonFunction& json, EnemyTemplate& tempData)
{
	ImGui::DragFloat("speed", &tempData.speed, 0.01f);
	ImGui::DragFloat("attackDistance", &tempData.attackDistance, 0.01f);
	ImGui::DragFloat("attackStartupTime", &tempData.attackStartupTime, 0.01f);
	ImGui::DragFloat("attackActiveTime", &tempData.attackActiveTime, 0.01f);
	ImGui::DragFloat("attackRecoveryTime", &tempData.attackRecoveryTime, 0.01f);
	ImGui::DragFloat("attackCoolTime", &tempData.attackCoolTime, 0.01f);
}

void EnemyAdjustItem::SetTemplateData(JsonFunction& json, EnemyTemplate& tempData)
{
	json.Set("speed", tempData.speed);
	json.Set("attackDistance", tempData.attackDistance);
	json.Set("attackStartupTime", tempData.attackStartupTime);
	json.Set("attackActiveTime", tempData.attackActiveTime);
	json.Set("attackRecoveryTime", tempData.attackRecoveryTime);
	json.Set("attackCoolTime", tempData.attackCoolTime);
}
