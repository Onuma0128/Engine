#include "EnemyAdjustItem.h"

#include "imgui.h"

void EnemyAdjustItem::LoadItems()
{
	/* ============================== Main ============================== */

	mainJson_.Init("EnemyMain");
	if (!mainJson_.Load()) {
		mainData_.spawnIndex = 0;
		mainJson_.Set("maxSpawn", 0);
		mainJson_.Set("nextWaveKillCount", 0);
		mainJson_.Set("startPosition", Vector3{});
		mainJson_.Set("colliderSize", 0.0f);
		mainJson_.Set("colliderOffset", Vector3{});
		mainJson_.Set("rayOffset", Vector3{});
		mainJson_.Set("margin", 1.0f);
		mainJson_.Set("searchUpdateTime", 1.0f);
		mainJson_.Set("maxDeadTimer", 1.0f);
		mainJson_.Set("kNockbackTimer", 1.0f);
		mainJson_.Set("particleTimer", 1.0f);
		mainJson_.Set("kNockbackScaleTimer", 1.0f);
		mainJson_.Set("kNockbackScale", 1.0f);
		mainJson_.Set("kNockbackVelocityY", 1.0f);
		mainJson_.Set("kNockbackAccelerY", 1.0f);
	} else {
		mainData_.spawnIndex = 0;
		mainData_.maxSpawn =			mainJson_.Get("maxSpawn", mainData_.maxSpawn);
		mainData_.nextWaveKillCount =	mainJson_.Get("nextWaveKillCount", mainData_.nextWaveKillCount);
		mainData_.startPosition =		mainJson_.Get("startPosition", mainData_.startPosition);
		mainData_.colliderSize =		mainJson_.Get("colliderSize", mainData_.colliderSize);
		mainData_.colliderOffset =		mainJson_.Get("colliderOffset", mainData_.colliderOffset);
		mainData_.rayOffset =			mainJson_.Get("rayOffset", mainData_.rayOffset);
		mainData_.margin =				mainJson_.Get("margin", mainData_.margin);
		mainData_.searchUpdateTime =	mainJson_.Get("searchUpdateTime", mainData_.searchUpdateTime);
		mainData_.maxDeadTimer =		mainJson_.Get("maxDeadTimer", mainData_.maxDeadTimer);
		mainData_.kNockbackTimer =		mainJson_.Get("kNockbackTimer", mainData_.kNockbackTimer);
		mainData_.particleTimer =		mainJson_.Get("particleTimer", mainData_.particleTimer);
		mainData_.kNockbackScaleTimer = mainJson_.Get("kNockbackScaleTimer", mainData_.kNockbackScaleTimer);
		mainData_.kNockbackScale =		mainJson_.Get("kNockbackScale", mainData_.kNockbackScale);
		mainData_.kNockbackVelocityY =	mainJson_.Get("kNockbackVelocityY", mainData_.kNockbackVelocityY);
		mainData_.kNockbackAccelerY =	mainJson_.Get("kNockbackAccelerY", mainData_.kNockbackAccelerY);
	}
	mainData_.debugIsSpawn = false;
	mainData_.debugSpline = false;

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
		rangedJson_.Set("planeSize", Vector3{});
		rangedJson_.Set("planeOffset", Vector3{});
	} else {
		rangedData_.tempData = GetTemplateData(rangedJson_);
		rangedData_.bulletSpeed = rangedJson_.Get("bulletSpeed", rangedData_.bulletSpeed);
		rangedData_.colliderSize = rangedJson_.Get("colliderSize", rangedData_.colliderSize);
		rangedData_.colliderOffset = rangedJson_.Get("colliderOffset", rangedData_.colliderOffset);
		rangedData_.planeSize = rangedJson_.Get("planeSize", rangedData_.planeSize);
		rangedData_.planeOffset = rangedJson_.Get("planeOffset", rangedData_.planeOffset);
	}

	/* ============================== ShieldBearer ============================== */

	shieldBearerJson_.Init("EnemyShieldBearer");
	if (!shieldBearerJson_.Load()) {
		SetJsonParameter(shieldBearerJson_);
		shieldBearerJson_.Set("attackColliderSize", 0.0f);
		shieldBearerJson_.Set("attackColliderOffset", Vector3{});
		shieldBearerJson_.Set("lerpSpeed", 0.0f);
		shieldBearerJson_.Set("shieldSize", Vector3{});
		shieldBearerJson_.Set("shieldOffset", Vector3{});
		shieldBearerJson_.Set("shieldColliderSize", Vector3{});
		shieldBearerJson_.Set("shieldColliderOffset", Vector3{});
	} else {
		shieldBearerData_.tempData = GetTemplateData(shieldBearerJson_);
		shieldBearerData_.attackColliderSize = shieldBearerJson_.Get("attackColliderSize", shieldBearerData_.attackColliderSize);
		shieldBearerData_.attackColliderOffset = shieldBearerJson_.Get("attackColliderOffset", shieldBearerData_.attackColliderOffset);
		shieldBearerData_.lerpSpeed = shieldBearerJson_.Get("lerpSpeed", shieldBearerData_.lerpSpeed);
		shieldBearerData_.shieldSize = shieldBearerJson_.Get("shieldSize", shieldBearerData_.shieldSize);
		shieldBearerData_.shieldOffset = shieldBearerJson_.Get("shieldOffset", shieldBearerData_.shieldOffset);
		shieldBearerData_.shieldColliderSize = shieldBearerJson_.Get("shieldColliderSize", shieldBearerData_.shieldColliderSize);
		shieldBearerData_.shieldColliderOffset = shieldBearerJson_.Get("shieldColliderOffset", shieldBearerData_.shieldColliderOffset);
	}

	/* ============================== RangedElite ============================== */

	rangedEliteJson_.Init("EnemyRangedElite");
	if (!rangedEliteJson_.Load()) {
		SetJsonParameter(rangedEliteJson_);
		rangedEliteJson_.Set("bulletSpeed", 0.0f);
		rangedEliteJson_.Set("bulletRadSpace", 0.0f);
		for (size_t i = 0; i < rangedEliteData_.planeSize.size(); ++i) {
			rangedEliteJson_.Set("planeSize" + std::to_string(i), Vector3{});
			rangedEliteJson_.Set("planeOffset" + std::to_string(i), Vector3{});
		}
	} else {
		rangedEliteData_.tempData = GetTemplateData(rangedEliteJson_);
		rangedEliteData_.bulletSpeed = rangedEliteJson_.Get("bulletSpeed", rangedEliteData_.bulletSpeed);
		rangedEliteData_.bulletRadSpace = rangedEliteJson_.Get("bulletRadSpace", rangedEliteData_.bulletRadSpace);
		for (size_t i = 0; i < rangedEliteData_.planeSize.size(); ++i) {
			rangedEliteData_.planeSize[i] = rangedEliteJson_.Get("planeSize" + std::to_string(i), rangedEliteData_.planeSize[i]);
			rangedEliteData_.planeOffset[i] = rangedEliteJson_.Get("planeOffset" + std::to_string(i), rangedEliteData_.planeOffset[i]);
		}
	}
}

void EnemyAdjustItem::Editor()
{

	ImGui::Begin("GameData");
	if (ImGui::TreeNode("EnemyParameters")) {
		ImGui::Separator();

		ImGui::Checkbox("isSpawn", &mainData_.debugIsSpawn);
		ImGui::Checkbox("debugSpline", &mainData_.debugSpline);

		std::vector<std::string>items = { "Melee","Ranged","ShieldBearer", "RangedElite" };
		// ブレンドを選択
		if (ImGui::BeginCombo("EnemyType", items[mainData_.spawnIndex].c_str())) {
			for (int i = 0; i < items.size(); ++i) {
				const bool is_selected = (mainData_.spawnIndex == i);
				if (ImGui::Selectable(items[i].c_str(), is_selected)) { mainData_.spawnIndex = i; }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}
		mainData_.nowSpawn = false;
		if (ImGui::Button("spawn")) {
			mainData_.nowSpawn = true;
		}

		/* ============================== Main ============================== */

		ImGui::PushItemWidth(150);
		if (ImGui::TreeNode("Main")) {

			ImGui::DragInt("maxSpawn", &mainData_.maxSpawn);
			ImGui::DragInt("nextWaveKillCount", &mainData_.nextWaveKillCount);
			ImGui::DragFloat3("startPosition", &mainData_.startPosition.x, 0.01f);
			ImGui::DragFloat("colliderSize", &mainData_.colliderSize, 0.01f);
			ImGui::DragFloat3("colliderOffset", &mainData_.colliderOffset.x, 0.01f);
			ImGui::DragFloat3("rayOffset", &mainData_.rayOffset.x, 0.01f);
			ImGui::DragFloat("margin", &mainData_.margin, 0.01f);
			ImGui::DragFloat("searchUpdateTime", &mainData_.searchUpdateTime, 0.01f);

			ImGui::DragFloat("maxDeadTimer", &mainData_.maxDeadTimer, 0.01f);
			ImGui::DragFloat("particleTimer", &mainData_.particleTimer, 0.01f);
			ImGui::DragFloat("kNockbackTimer", &mainData_.kNockbackTimer, 0.01f);
			ImGui::DragFloat("kNockbackScaleTimer", &mainData_.kNockbackScaleTimer, 0.01f);
			ImGui::DragFloat("kNockbackScale", &mainData_.kNockbackScale, 0.01f);
			ImGui::DragFloat("kNockbackVelocityY", &mainData_.kNockbackVelocityY, 0.01f);
			ImGui::DragFloat("kNockbackAccelerY", &mainData_.kNockbackAccelerY, 0.01f);
			if (ImGui::Button("Save")) {
				mainJson_.Set("maxSpawn", mainData_.maxSpawn);
				mainJson_.Set("nextWaveKillCount", mainData_.nextWaveKillCount);
				mainJson_.Set("startPosition", mainData_.startPosition);
				mainJson_.Set("colliderSize", mainData_.colliderSize);
				mainJson_.Set("colliderOffset", mainData_.colliderOffset);
				mainJson_.Set("rayOffset", mainData_.rayOffset);
				mainJson_.Set("margin", mainData_.margin);
				mainJson_.Set("searchUpdateTime", mainData_.searchUpdateTime);

				mainJson_.Set("maxDeadTimer", mainData_.maxDeadTimer);
				mainJson_.Set("kNockbackTimer", mainData_.kNockbackTimer);
				mainJson_.Set("particleTimer", mainData_.particleTimer);
				mainJson_.Set("kNockbackScaleTimer", mainData_.kNockbackScaleTimer);
				mainJson_.Set("kNockbackScale", mainData_.kNockbackScale);
				mainJson_.Set("kNockbackVelocityY", mainData_.kNockbackVelocityY);
				mainJson_.Set("kNockbackAccelerY", mainData_.kNockbackAccelerY);
				mainJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();

		/* ============================== Melee ============================== */

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
			ImGui::DragFloat3("planeSize", &rangedData_.planeSize.x, 0.01f);
			ImGui::DragFloat3("planeOffset", &rangedData_.planeOffset.x, 0.01f);
			if (ImGui::Button("Save")) {
				SetTemplateData(rangedJson_, rangedData_.tempData);
				rangedJson_.Set("bulletSpeed", rangedData_.bulletSpeed);
				rangedJson_.Set("colliderSize", rangedData_.colliderSize);
				rangedJson_.Set("colliderOffset", rangedData_.colliderOffset);
				rangedJson_.Set("planeSize", rangedData_.planeSize);
				rangedJson_.Set("planeOffset", rangedData_.planeOffset);
				rangedJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();

		/* ============================== ShieldBearer ============================== */

		if (ImGui::TreeNode("ShieldBearer")) {

			SetImGuiData(shieldBearerJson_, shieldBearerData_.tempData);
			ImGui::DragFloat("attackColliderSize", &shieldBearerData_.attackColliderSize, 0.01f);
			ImGui::DragFloat3("attackColliderOffset", &shieldBearerData_.attackColliderOffset.x, 0.01f);
			ImGui::DragFloat("lerpSpeed", &shieldBearerData_.lerpSpeed, 0.01f);
			ImGui::DragFloat3("shieldSize", &shieldBearerData_.shieldSize.x, 0.01f);
			ImGui::DragFloat3("shieldOffset", &shieldBearerData_.shieldOffset.x, 0.01f);
			ImGui::DragFloat3("shieldColliderSize", &shieldBearerData_.shieldColliderSize.x, 0.01f);
			ImGui::DragFloat3("shieldColliderOffset", &shieldBearerData_.shieldColliderOffset.x, 0.01f);
			if (ImGui::Button("Save")) {
				SetTemplateData(shieldBearerJson_, shieldBearerData_.tempData);
				shieldBearerJson_.Set("attackColliderSize", shieldBearerData_.attackColliderSize);
				shieldBearerJson_.Set("attackColliderOffset", shieldBearerData_.attackColliderOffset);
				shieldBearerJson_.Set("lerpSpeed", shieldBearerData_.lerpSpeed);
				shieldBearerJson_.Set("shieldSize", shieldBearerData_.shieldSize);
				shieldBearerJson_.Set("shieldOffset", shieldBearerData_.shieldOffset);
				shieldBearerJson_.Set("shieldColliderSize", shieldBearerData_.shieldColliderSize);
				shieldBearerJson_.Set("shieldColliderOffset", shieldBearerData_.shieldColliderOffset);
				shieldBearerJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();

		/* ============================== RangedElite ============================== */

		if (ImGui::TreeNode("RangedElite")) {

			SetImGuiData(rangedEliteJson_, rangedEliteData_.tempData);
			ImGui::DragFloat("bulletSpeed", &rangedEliteData_.bulletSpeed, 0.01f);
			ImGui::DragFloat("bulletRadSpace", &rangedEliteData_.bulletRadSpace, 0.01f);
			for (size_t i = 0; i < rangedEliteData_.planeSize.size(); ++i) {
				std::string labelSize = "planeSize" + std::to_string(i);
				std::string labelOffset = "planeOffset" + std::to_string(i);
				ImGui::DragFloat3(labelSize.c_str(), &rangedEliteData_.planeSize[i].x, 0.01f);
				ImGui::DragFloat3(labelOffset.c_str(), &rangedEliteData_.planeOffset[i].x, 0.01f);
			}
			if (ImGui::Button("Save")) {
				SetTemplateData(rangedEliteJson_, rangedEliteData_.tempData);
				rangedEliteJson_.Set("bulletSpeed", rangedEliteData_.bulletSpeed);
				rangedEliteJson_.Set("bulletRadSpace", rangedEliteData_.bulletRadSpace);
				for (size_t i = 0; i < rangedEliteData_.planeSize.size(); ++i) {
					rangedEliteJson_.Set("planeSize" + std::to_string(i), rangedEliteData_.planeSize[i]);
					rangedEliteJson_.Set("planeOffset" + std::to_string(i), rangedEliteData_.planeOffset[i]);
				}
				rangedEliteJson_.Save();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();

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
	ImGui::DragFloat("speed", &tempData.speed, 0.01f, 0.0f, 100.0f);
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
