#include "EnemyAdjustItem.h"

#include "imgui.h"

void EnemyAdjustItem::LoadItems()
{
	/* ============================== Melee ============================== */

	meleeJson_.Init("EnemyMelee");
	if (!meleeJson_.Load()) {
		SetJsonParameter(meleeJson_);
	} else {
		meleeData_.tempData = GetTemplateData(meleeJson_);
	}

	/* ============================== Ranged ============================== */

	rangedJson_.Init("EnemyRanged");
	if (!rangedJson_.Load()) {
		SetJsonParameter(rangedJson_);
	} else {
		rangedData_.tempData = GetTemplateData(rangedJson_);
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
	} else {
		rangedEliteData_.tempData = GetTemplateData(rangedEliteJson_);
	}
}

void EnemyAdjustItem::Editor()
{
	/* ============================== Melee ============================== */

	ImGui::Begin("EnemyParameters");

	if (ImGui::TreeNode("Melee")) {

		SetImGuiData(meleeJson_, meleeData_.tempData);
		if (ImGui::Button("Save")) {
			SetTemplateData(meleeJson_, meleeData_.tempData);
			meleeJson_.Save();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

	/* ============================== Ranged ============================== */

	if (ImGui::TreeNode("Ranged")) {

		SetImGuiData(rangedJson_, rangedData_.tempData);
		if (ImGui::Button("Save")) {
			SetTemplateData(rangedJson_, rangedData_.tempData);
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
		if (ImGui::Button("Save")) {
			SetTemplateData(rangedEliteJson_, rangedEliteData_.tempData);
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
}

EnemyTemplate EnemyAdjustItem::GetTemplateData(JsonFunction json)
{
	EnemyTemplate data{};
	data.speed = json.Get("speed", data.speed);
	data.attackDistance = json.Get("attackDistance", data.attackDistance);
	data.attackStartupTime = json.Get("attackStartupTime", data.attackStartupTime);
	data.attackActiveTime = json.Get("attackActiveTime", data.attackActiveTime);
	data.attackRecoveryTime = json.Get("attackRecoveryTime", data.attackRecoveryTime);
	return data;
}

void EnemyAdjustItem::SetImGuiData(JsonFunction& json, EnemyTemplate& tempData)
{
	ImGui::DragFloat("speed", &tempData.speed, 0.01f);
	ImGui::DragFloat("attackDistance", &tempData.attackDistance, 0.01f);
	ImGui::DragFloat("attackStartupTime", &tempData.attackStartupTime, 0.01f);
	ImGui::DragFloat("attackActiveTime", &tempData.attackActiveTime, 0.01f);
	ImGui::DragFloat("attackRecoveryTime", &tempData.attackRecoveryTime, 0.01f);
}

void EnemyAdjustItem::SetTemplateData(JsonFunction& json, EnemyTemplate& tempData)
{
	json.Set("speed", tempData.speed);
	json.Set("attackDistance", tempData.attackDistance);
	json.Set("attackStartupTime", tempData.attackStartupTime);
	json.Set("attackActiveTime", tempData.attackActiveTime);
	json.Set("attackRecoveryTime", tempData.attackRecoveryTime);
}
